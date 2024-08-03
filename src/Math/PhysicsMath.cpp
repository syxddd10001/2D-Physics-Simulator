#include "PhysicsMath.hpp"

#define DEBUG 1
#if DEBUG == 1
#define DEBUG_PRINT(format, ...) printf(format, ##__VA_ARGS__)
#else
#define DEBUG_PRINT(format, ...) // Do nothing //
#endif


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

std::pair<float, float> calculateVelocity( Object* object, float delta_time, const double force ){
    std::pair<float, float> velocity = object->getVelocity();
    std::pair<float, float> position = object->getPosition();

    velocity.first += velocity.first * force * delta_time; 
    velocity.second += velocity.second * force * delta_time;
    
    position.first += velocity.first * delta_time; 
    position.second += velocity.second * delta_time;
    
    if ( std::sqrt( velocity.first*velocity.first + velocity.second * velocity.second ) < 0.01f ) 
    velocity = std::make_pair( 0.0f, 0.0f );
    
    object->setVelocity( velocity );

    return position;
}
//
std::pair<float, float> calculateVelocity( Object* object, float delta_time ){
    std::pair<float, float> velocity = object->getVelocity();
    std::pair<float, float> position = object->getPosition();

    velocity.first += velocity.first  * delta_time; 
    velocity.second += velocity.second * delta_time;
    
    position.first += velocity.first * delta_time; 
    position.second += velocity.second * delta_time;
    
    if ( std::sqrt( velocity.first*velocity.first + velocity.second * velocity.second ) < 0.01f ) 
    velocity = std::make_pair( 0.0f, 0.0f );
    
    object->setVelocity( velocity );

    return position;
}

double applyForce( Object* object1, Object* object2 ) {
    double distance = calculateDistance(object1->getPosition(), object2->getPosition());
    double mass_1 = object1->getMass();
    double mass_2 = object2->getMass();
    std::cout << std::setprecision(13);
    std::cout << "dist:" << distance << std::endl;
    double force =  ( GRAVITATIONAL_CONSTANT * mass_1 * mass_2 ) / ( distance*distance );
    
    return force;

}

std::pair<double, double> calculateAcceleration(Object* object, double force) {
    double mass = object->getMass();
    double acceleration = force / mass;
    return std::pair<double, double>(acceleration, acceleration); // Assuming equal acceleration in both directions
}
