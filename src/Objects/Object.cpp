#pragma once
#include <Object.hpp>
#include <iostream>
#include <utility>

Object::Object( float mass, float pos_x, float pos_y ) 
: mass( mass ), position_x( pos_x ), position_y( pos_y )
{
    this->setVelocity( std::pair<float, float> { 0.f, 0.f } );
}

Object::Object( )
{
    
}

Object::~Object( ){
    std::cout << "Object destroyed\n";

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

void Object::setPosition( const std::pair<float, float> pos ){
    position_x = pos.first;
    position_y = pos.second;
    
}

std::pair<float, float> Object::getPosition(){
    return std::pair<float, float> ( position_x, position_y );
}

std::pair<float, float> Object::getVelocity(){
    return std::pair<float, float> ( velocity_x, velocity_y );
}

std::pair<float, float> Object::setVelocity( const float vel_x, const float vel_y ){
    this->velocity_x = vel_x; 
    this->velocity_y = vel_y; 
    return std::pair<float, float> ( velocity_x, velocity_y );
}

void Object::setVelocity( const std::pair<float, float> vel ){
    velocity_x = vel.first;
    velocity_y = vel.second;
}

void Object::setAcceleration( const std::pair<float, float> acc ){
    acceleration_x = acc.first;
    acceleration_y = acc.second;
}

std::pair<float, float> Object::getAcceleration(){
    return std::pair<float, float> ( acceleration_x, acceleration_y );
}

std::pair<float, float> Object::setAcceleration( const float acc_x, const float acc_y ){
    this->acceleration_x = acc_x;
    this->acceleration_y = acc_y;
    return std::pair<float, float> ( acceleration_x, acceleration_y );
}

void Object::setID( int id ){
    this->object_id = id;
}

int Object::getID(){
    return this->object_id;
}

bool Object::mouseOnObject( const sf::Vector2f vector ){
    return false;
}