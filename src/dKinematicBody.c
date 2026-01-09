#include <stdio.h>
#include <stdlib.h>

#include "Daedalus.h"

dKinematicBody_t* d_KinematicBodyCreate( const dVec3_t position,
                                        const float mass,
                                        const float max_speed,
                                        const float max_force )
{
  dKinematicBody_t* new_body = malloc( sizeof( dKinematicBody_t ) );
  if ( new_body == NULL ) return NULL;

  new_body->position = position;
  new_body->mass = mass;
  new_body->max_speed = max_speed;
  new_body->max_force = max_force;
  new_body->acceleration = (dVec3_t){0};
  new_body->velocity = (dVec3_t){0};

  return new_body;
}

void d_KinematicBodyUpdate( dKinematicBody_t* body )
{
  d_AddTwoVec3f( &body->velocity, body->velocity, body->acceleration );
  d_LimitVec3f( &body->velocity, body->velocity, body->max_speed );
  d_AddTwoVec3f( &body->position, body->position, body->velocity );
  d_ScaleMultiplyVec3f(&body->acceleration, body->acceleration, 0 );
}

void d_KinematicBodyApplyForce( dKinematicBody_t* body, dVec3_t force )
{
  dVec3_t accumulated_force;
  d_ScaleDivideVec3f( &accumulated_force, force, body->mass );
  d_AddTwoVec3f( &body->acceleration, body->acceleration, accumulated_force );
}

void d_KinematicBodySeek( dKinematicBody_t* body, dVec3_t target )
{
  dVec3_t seek = {0}, desired = {0};
  d_SubTwoVec3f( &desired, target, body->position );
  d_SetMagVec3f( &desired, desired, body->max_speed );
  d_SubTwoVec3f( &seek, desired, body->velocity );
  d_LimitVec3f( &seek, seek, body->max_force );
  d_KinematicBodyApplyForce( body, seek );
}

