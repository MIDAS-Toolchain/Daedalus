#include <stdio.h>
#include <stdlib.h>

#include "Daedalus.h"

dQuadTree_t* d_QuadTreeCreate( dRectf_t boundary )
{
  dQuadTree_t* new_tree = malloc( sizeof( dQuadTree_t ) );
  if ( new_tree == NULL )
  {
    printf( "Failed to allocate memory for new_tree: %s, %d", __FILE__, __LINE__ );
    return NULL;
  }

  new_tree->rect = boundary;
  new_tree->count = 0;
  new_tree->is_divided = 0;
  new_tree->nw = new_tree->ne = new_tree->sw = new_tree->se = NULL;

  for ( int i = 0; i < QT_CAPACITY; i++ )
  {
    new_tree->points[i] = (dVec3_t){ .x = 0, .y = 0, .z = 0 };
  }

  return new_tree;
}

uint8_t d_QuadTreeContains( dRectf_t boundary, dVec3_t p )
{
  return ( p.x >= boundary.x - boundary.w &&
           p.x <= boundary.x + boundary.w && 
           p.y >= boundary.y - boundary.h &&
           p.y <= boundary.y + boundary.h );
}

uint8_t d_QuadTreeIntersects(  dRectf_t range, dRectf_t boundary )
{
  return !( range.x - range.w > boundary.x - boundary.w ||
            range.x + range.w < boundary.x + boundary.w ||
            range.y - range.h > boundary.y - boundary.h ||
            range.y + range.h < boundary.y + boundary.h );
}

void d_QuadTreeSubdivide( dQuadTree_t* node )
{
  if ( node->is_divided ) return;

  float x = node->rect.x;
  float y = node->rect.y;
  float w = node->rect.w / 2.0f;
  float h = node->rect.h / 2.0f;

  dRectf_t nw_bound = { x - w, y - h, w, h};
  node->nw = d_QuadTreeCreate( nw_bound );
  
  dRectf_t ne_bound = { x + w, y - h, w, h};
  node->ne = d_QuadTreeCreate( ne_bound );
  
  dRectf_t sw_bound = { x - w, y + h, w, h};
  node->sw = d_QuadTreeCreate( sw_bound );
  
  dRectf_t se_bound = { x + w, y + h, w, h};
  node->se = d_QuadTreeCreate( se_bound );

  node->is_divided = 1;

  for ( int i = 0; i < node->count; i++ )
  {
    d_QuadTreeInsert( node, node->points[i] );
    node->points[i] = (dVec3_t){ .x = 0, .y = 0, .z = 0 };
  }

  node->count = 0;
}

uint8_t d_QuadTreeInsert( dQuadTree_t* node, dVec3_t p )
{
  if ( !d_QuadTreeContains( node->rect, p ) ) 
  {
    return 0;
  }

  if ( !node->is_divided && node->count < QT_CAPACITY )
  {
    node->points[node->count] = p;
    node->count++;
    return 1;
  }

  if ( !node->is_divided && node->count == QT_CAPACITY )
  {
    d_QuadTreeSubdivide( node );
  }

  if ( node->is_divided )
  {
    if ( d_QuadTreeInsert( node->nw, p) ) return 1;
    if ( d_QuadTreeInsert( node->ne, p) ) return 1;
    if ( d_QuadTreeInsert( node->sw, p) ) return 1;
    if ( d_QuadTreeInsert( node->se, p) ) return 1;
  }

  return 0;
}

dArray_t* d_QuadTreeQuery( dQuadTree_t* root, dRectf_t range )
{
  dArray_t* found = d_ArrayInit( 10, sizeof( dVec3_t ) );

  if ( !d_QuadTreeIntersects( range, root->rect ) )
  {
    return NULL;
  }
  
  else
  {
    for ( int i = 0; i < root->count; i++ )
    {
      dVec3_t p = root->points[i];
      if ( d_QuadTreeContains( range, p ) )
      {

        d_ArrayAppend( found, &p );
      }
    }

    if ( root->is_divided )
    {
      d_QuadTreeQuery( root->ne, range );
      d_QuadTreeQuery( root->nw, range );
      d_QuadTreeQuery( root->se, range );
      d_QuadTreeQuery( root->sw, range );
    }
  }

  return found;
}

void d_QuadTreeFree( dQuadTree_t* node )
{
  if ( node == NULL ) return;

  if ( node->is_divided )
  {
    d_QuadTreeFree( node->nw );
    d_QuadTreeFree( node->ne );
    d_QuadTreeFree( node->sw );
    d_QuadTreeFree( node->se );
  }

  free( node );
}

