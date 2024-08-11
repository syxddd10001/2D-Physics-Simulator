#include <Object.hpp>

//
Object::Object( float mass, float pos_x, float pos_y ) 
: mass( mass ), position_x( pos_x ), position_y( pos_y )
{
  this->setVelocity( Vec2 { 0.f, 0.f } );
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

void Object::setPosition( const Vec2 pos ){
  position_x = pos.x;
  position_y = pos.y;
    
}

void Object::setMass( const float mass ){
  this->mass = mass;
}

Vec2 Object::getPosition(){
  return Vec2 ( position_x, position_y );
}

Vec2 Object::getVelocity(){
  return Vec2 ( velocity_x, velocity_y );
}

Vec2 Object::getCenter() {
  return Vec2 { 0.f, 0.f}; 
}

Vec2 Object::setVelocity( const float vel_x, const float vel_y ){
  this->velocity_x = vel_x; 
  this->velocity_y = vel_y; 
  return Vec2 ( velocity_x, velocity_y );
}

void Object::setVelocity( const Vec2 vel ){
  velocity_x = vel.x;
  velocity_y = vel.y;
}

void Object::setAcceleration( const Vec2 acc ){
  acceleration_x = acc.x;
  acceleration_y = acc.y;
}

Vec2 Object::getAcceleration(){
  return Vec2 ( acceleration_x, acceleration_y );
}

Vec2 Object::setAcceleration( const float acc_x, const float acc_y ){
  this->acceleration_x = acc_x;
  this->acceleration_y = acc_y;
  return Vec2 ( acceleration_x, acceleration_y );
}

void Object::setID( int id ){
  this->object_id = id;
}

Vec2 Object::getSize(){
  return Vec2 ( 0.0f, 0.0f );
}

int Object::getID(){
  return this->object_id;
}

void Object::setQueryBox( AbstractBox<float> refbox ){
  this->queryBox = refbox;
}

AbstractBox<float> Object::getQueryBox(){
  return this->queryBox;
}

bool Object::mouseOnObject( const sf::Vector2f vector ){
  return false;
}

Object::ObjectType Object::getType(){
  return Object::NONE;
}