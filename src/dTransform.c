#include <stdio.h>
#include <stdlib.h>

#include "Daedalus.h"

dTransform_t* d_TransformCreate( dVec3_t position,
                                 dVec3_t scale,
                                 double rotation )
{
  dTransform_t* new_trans = malloc( sizeof( dTransform_t ) );
  if ( new_trans == NULL )
  {
    printf( "Failed to allocate memory for new_trans: %s, %d\n",
            __FILE__, __LINE__ );
    return NULL;
  }

  new_trans->position = position;
  new_trans->rotation = rotation;
  new_trans->scale    = scale;

  return new_trans;
}

