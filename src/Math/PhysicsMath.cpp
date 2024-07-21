#include <Objects/Object.hpp>
#include "PhysicsMath.hpp"

#include <utility>
#include <cmath>



std::pair<float, float> pairAdd( std::pair<float,float> pair1, std::pair<float,float> pair2 ){
    return std::pair<float, float>( pair1.first+pair2.first, pair1.second+pair2.second );
}    

std::pair<float, float> pairSubtract( std::pair<float,float> pair1, std::pair<float,float> pair2 ){
    return std::pair<float, float>( pair1.first-pair2.first, pair1.second-pair2.second );
}

std::pair<float, float> pairMultiply( std::pair<float,float> pair1, std::pair<float,float> pair2 ){
    return std::pair<float, float>( pair1.first*pair2.first, pair1.second*pair2.second );
}

float calculateDistance( std::pair<float,float> pos1, std::pair<float,float> pos2 ){
    std::pair<float, float> difference = pairSubtract( pos1, pos2 );
    return std::sqrt( difference.first * difference.first + difference.second * difference.second );
}

float dotProduct( std::pair<float,float> vectorA, std::pair<float, float> vectorB ){
    return vectorA.first*vectorB.first + vectorA.second*vectorB.second;
}

std::pair<float, float> calculateVelocity( Object* object, float delta_time ){
    std::pair<float, float> acceleration = object->getAcceleration();
    std::pair<float, float> velocity = object->getVelocity();
    std::pair<float, float> position = object->getPosition();
    
    float friction = -0.7f;

    acceleration.first = velocity.first * friction; 
    acceleration.second = velocity.second * friction; 
    
    velocity.first += acceleration.first * delta_time; 
    velocity.second += acceleration.second * delta_time;
    
    position.first += velocity.first * delta_time; 
    position.second += velocity.second * delta_time;
    
    if ( std::sqrt( velocity.first*velocity.first + velocity.second * velocity.second ) < 0.01f ) 
    velocity = std::make_pair( 0.0f, 0.0f );
    
    object->setVelocity( velocity );

    return position;
}
