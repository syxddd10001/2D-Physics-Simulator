#include <Objects/Object.hpp>
#include <iostream>
#include <utility>

Object::Object( float mass, float pos_x, float pos_y ) 
: mass( mass ), position_x( pos_x ), position_y( pos_y )
{
    
}

Object::Object( )
{
    
}

Object::~Object( ){
    std::cout << "Object destroyed\n";
    delete shape;
}

void Object::print_info( ){
    std::cout << "Mass: " << mass << "\nPosition: (" << position_x << ", " << position_y << ")\n";
    std::cout << "Velocity: (" << velocity_x << ", " << velocity_y << ")\n"; 
 
}

float Object::getMass( ){
    return mass;
} 

sf::Shape* Object::getShape( ){
    return shape;
}

void Object::setPosition( std::pair<float, float> pos ){
    position_x = pos.first;
    position_y = pos.second;
    
}

std::pair<float, float> Object::getPosition(){
    return std::pair<float, float> ( position_x, position_y );
}

std::pair<float, float> Object::getVelocity(){
    return std::pair<float, float> ( velocity_x, velocity_y );
}

std::pair<float, float> Object::setVelocity( float vel_x, float vel_y ){
    this->velocity_x = vel_x; 
    this->velocity_y = vel_y; 
    return std::pair<float, float> ( velocity_x, velocity_y );
}

void Object::setVelocity( std::pair<float, float> vel ){
    velocity_x = vel.first;
    velocity_y = vel.second;
}

void Object::setAcceleration( std::pair<float, float> acc ){
    acceleration_x = acc.first;
    acceleration_y = acc.second;
}

std::pair<float, float> Object::getAcceleration(){
    return std::pair<float, float> ( acceleration_x, acceleration_y );
}

std::pair<float, float> Object::setAcceleration( float acc_x, float acc_y ){
    this->acceleration_x = acc_x;
    this->acceleration_y = acc_y;
    return std::pair<float, float> ( acceleration_x, acceleration_y );
}

bool Object::mouseOnObject( sf::Vector2f vector ){
    return false;
}