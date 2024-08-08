#pragma once
#include <utility>
#include <math.h>
#include <iomanip>
#include <utility>
#include <cmath>
#include <iostream>

#include "Object.hpp"
#include "Vector2.hpp"

using Vec2 = syxd::Vector2<float>;

const long double GRAVITATIONAL_CONSTANT = 6.674010551359 * pow(10,-11); // N m^2/kg^2

point pairAdd( point pair1, point pair2 ) ;
point pairSubtract( point pair1, point pair2 );
point pairMultiply( point pair1, point pair2 );

float calculateDistance( Vec2 pos1, Vec2 pos2 );

Vec2 calculateVelocity( Object* object, float delta_time, const Vec2 acceleration );
float dotProduct( point vectorA, point vectorB );
Vec2 calculateAcceleration( Object* object, float delta_time, Vec2 velocity );
Vec2 applyForce( Object* this_object, Object* other_object, const float delta_time );
Vec2 normalizeVector( const Vec2 p );