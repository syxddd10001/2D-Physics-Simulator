#pragma once
#include <utility>
#include <math.h>
#include <iomanip>
#include <utility>
#include <cmath>
#include <iostream>

#include "Object.hpp"

const long double GRAVITATIONAL_CONSTANT = 6.674010551359 * pow(10,-11); // N m^2/kg^2

using point = std::pair<float, float>;

point pairAdd( point pair1, point pair2 ) ;
point pairSubtract( point pair1, point pair2 );
point pairMultiply( point pair1, point pair2 );

float calculateDistance( point pos1, point pos2 );
point calculateVelocity( Object* object, float delta_time, const double force );
point calculateVelocity( Object* object, float delta_time );
double applyForce( Object* object1, Object* object2 );
float dotProduct( point vectorA, point vectorB );
point calculateAcceleration( Object* object, float delta_time, point velocity );
