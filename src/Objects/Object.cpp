#include <Object.hpp>


Object::Object( float mass, float pos_x, float pos_y ) 
: mass( mass ), position_x( pos_x ), position_y( pos_y )
{
  this->setVelocity( point { 0.f, 0.f } );
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

void Object::setPosition( const point pos ){
  position_x = pos.first;
  position_y = pos.second;
    
}

void Object::setMass( const float mass ){
  this->mass = mass;
}

point Object::getPosition(){
  return point ( position_x, position_y );
}

point Object::getVelocity(){
  return point ( velocity_x, velocity_y );
}

point Object::setVelocity( const float vel_x, const float vel_y ){
  this->velocity_x = vel_x; 
  this->velocity_y = vel_y; 
  return point ( velocity_x, velocity_y );
}

void Object::setVelocity( const point vel ){
  velocity_x = vel.first;
  velocity_y = vel.second;
}

void Object::setAcceleration( const point acc ){
  acceleration_x = acc.first;
  acceleration_y = acc.second;
}

point Object::getAcceleration(){
  return point ( acceleration_x, acceleration_y );
}

point Object::setAcceleration( const float acc_x, const float acc_y ){
  this->acceleration_x = acc_x;
  this->acceleration_y = acc_y;
  return point ( acceleration_x, acceleration_y );
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