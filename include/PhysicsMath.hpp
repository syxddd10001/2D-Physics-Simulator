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

std::pair<float, float> pairAdd( std::pair<float,float> pair1, std::pair<float,float> pair2 ) ;
std::pair<float, float> pairSubtract( std::pair<float, float> pair1, std::pair<float, float> pair2 );
std::pair<float, float> pairMultiply( std::pair<float,float> pair1, std::pair<float,float> pair2 );

float calculateDistance( std::pair<float,float> pos1, std::pair<float,float> pos2 );
std::pair<float, float> calculateVelocity( Object* object, float delta_time, const double force );
std::pair<float, float> calculateVelocity( Object* object, float delta_time );
double applyForce( Object* object1, Object* object2 );
float dotProduct( std::pair<float,float> vectorA, std::pair<float,float> vectorB );
std::pair<double, double> calculateAcceleration( Object* object, double force );
