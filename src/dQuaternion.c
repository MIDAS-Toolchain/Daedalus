#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "Daedalus.h"

dQuaternion_t d_QuaternionConjugate( dQuaternion_t q )
{
  dQuaternion_t result = {
    .w = q.w,
    .x = -q.x,
    .y = -q.y,
    .z = -q.z,
  };

  return result;
}

// Function to multiply two quaternions ( q * p )
// w' = q.w*p.w - q.x*p.x - q.y*p.y - q.z*p.z
// x' = q.w*p.x - q.x*p.w - q.y*p.z - q.z*p.y
// y' = q.w*p.y - q.x*p.z - q.y*p.w - q.z*p.x
// z' = q.w*p.z - q.x*p.y - q.y*p.x - q.z*p.w
dQuaternion_t d_QuaternionMultiply( dQuaternion_t q, dQuaternion_t p )
{
  dQuaternion_t result;
  
  result.w = q.w*p.w - q.x*p.x - q.y*p.y - q.z*p.z;
  result.x = q.w*p.x - q.x*p.w - q.y*p.z - q.z*p.y;
  result.y = q.w*p.y - q.x*p.z - q.y*p.w - q.z*p.x;
  result.z = q.w*p.z - q.x*p.y - q.y*p.x - q.z*p.w;

  return result;
}

// Function to create a unit rotation quaternion from an axis and an angle ( in radians )
// assumes axis vector is a unit vector
dQuaternion_t d_QuaternionCreateRotation( float angle, dVec3_t u )
{
  dQuaternion_t q;

  float half_angle = angle / 2.0f;
  float sin_half = sinf( half_angle );

  q.w = cosf( half_angle );

  q.x = u.x * sin_half;
  q.y = u.y * sin_half;
  q.z = u.z * sin_half;
  
  return q;
}

// Function to rotate a vector using a quaternion
// the rotation is calculated using the formula: p' = q * p * q_conjugate
dVec3_t d_QuaternionRotateVector( dVec3_t v, dQuaternion_t q_rot )
{
  dQuaternion_t p = {
    .w = 0.0f,
    .x = v.x,
    .y = v.y,
    .z = v.z,
  };

  dQuaternion_t q_conj = d_QuaternionConjugate( q_rot );
  dQuaternion_t temp = d_QuaternionMultiply( q_rot, p );
  dQuaternion_t p_prime = d_QuaternionMultiply( temp, q_conj );

  dVec3_t v_rotated = {
    .x = p_prime.x,
    .y = p_prime.y,
    .z = p_prime.z,
  };

  return v_rotated;
}

// Function to convert a quaternion (rotating around Z-axis) to degrees
double d_QuaternionInDegrees( dQuaternion_t q )
{
  //Angle (radians) = 2 * atan2( sin( theta / 2 ), cos( theta / 2 ) )
  double angle_rad = 2.0 * atan2( q.z, q.w );
  
  double angle_deg = angle_rad * 180.0f / D_PI;

  if ( angle_deg < 0 )
  {
    angle_deg += 360.0f;
  }

  return angle_deg;
}

