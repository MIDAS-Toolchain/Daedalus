#include <stdio.h>
#include <stdlib.h>

#include "Daedalus.h"

dKinematicBody_t* d_KinematicBodyCreate( dVec3_t gravity, dVec3_t velocity,
                                         dVec3_t force, float mass,
                                         float max_speed, float max_force,
                                         uint8_t takes_gravity,
                                         float static_friction,
                                         float dynamic_friction,
                                         float restitution )
{
  dKinematicBody_t* new_body = malloc( sizeof( dKinematicBody_t ) );
  if ( new_body == NULL )
  {
    printf( "Failed to allocate memory for new_body: %s, %d\n",
            __FILE__, __LINE__ );
    return NULL;
  }

  new_body->gravity = gravity;
  new_body->velocity = velocity;
  new_body->force = force;
  new_body->mass = mass;
  new_body->max_speed = max_speed;
  new_body->max_force = max_force;
  new_body->takes_gravity = takes_gravity;
  new_body->static_friction = static_friction;
  new_body->dynamic_friction = dynamic_friction;
  new_body->restitution = restitution;

  return new_body;
}

void d_KinematicBodyUpdate( dKinematicBody_t* body, dTransform_t* trans,
                            float dt )
{
  dVec3_t acceleration = {0, 0, 0};
  d_ScaleDivideVec3f( &acceleration, body->force, body->mass );
  dVec3_t delta_velo = {0, 0, 0};
  d_ScaleMultiplyVec3f( &delta_velo, acceleration, dt );
  d_AddTwoVec3f( &body->velocity, body->velocity, delta_velo );
  //d_LimitVec3f( &body->velocity, body->velocity, body->max_speed );
  
  dVec3_t new_position = {0, 0, 0};
  d_ScaleMultiplyVec3f( &new_position, body->velocity, dt );
  d_AddTwoVec3f( &trans->position, trans->position, new_position );
  body->force = (dVec3_t){0, 0, 0};
}

void d_KinematicBodyApplyForce( dKinematicBody_t* body, dVec3_t force )
{
  dVec3_t accumulated_force = { 0, 0, 0 };
  d_ScaleMultiplyVec3f( &accumulated_force, force, body->mass );
  d_AddTwoVec3f( &body->force, body->force, accumulated_force );
  //body->velocity = (dVec3_t){0};
}

void d_KinematicBodySeek( dKinematicBody_t* body, dTransform_t* trans,
                          dVec3_t target )
{
  dVec3_t seek = {0}, desired = {0};
  d_SubTwoVec3f( &desired, target, trans->position );
  d_LimitVec3f( &desired, desired, body->max_speed );
  d_SubTwoVec3f( &seek, desired, body->velocity );
  d_LimitVec3f( &seek, seek, body->max_force );
  d_KinematicBodyApplyForce( body, seek );
}

