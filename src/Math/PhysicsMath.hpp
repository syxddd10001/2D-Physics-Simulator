#pragma once
#include <Objects/Object.hpp>
#include <utility>

using point = std::pair<float, float>;

std::pair<float, float> pairAdd( std::pair<float,float> pair1, std::pair<float,float> pair2 ) ;
std::pair<float, float> pairSubtract( std::pair<float, float> pair1, std::pair<float, float> pair2 );
std::pair<float, float> pairMultiply( std::pair<float,float> pair1, std::pair<float,float> pair2 );

float calculateDistance( std::pair<float,float> pos1, std::pair<float,float> pos2 );
std::pair<float, float> calculateVelocity( Object* object, float delta_time );
float dotProduct( std::pair<float,float> vectorA, std::pair<float,float> vectorB );
