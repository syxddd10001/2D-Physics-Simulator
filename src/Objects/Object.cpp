#include <Object.hpp>

//
Object::Object( float mass, float pos_x, float pos_y ) noexcept
: m_mass( mass ), m_position_x( pos_x ), m_position_y( pos_y )
{
  this->setVelocity( Vec2 { 0.f, 0.f } );
}

void Object::print_info( ) {
  std::cout << "Mass: " << m_mass << "\nPosition: (" << m_position_x << ", " << m_position_y << ")\n";
  std::cout << "Velocity: (" << m_velocity_x << ", " << m_velocity_y << ")\n"; 
 
}

float Object::getMass( ) {
  return m_mass;
} 

std::shared_ptr<sf::Shape> Object::getShape( ) {
  return m_shape;
}

void Object::setPosition( const Vec2 pos ) {
  m_position_x = pos.x;
  m_position_y = pos.y;
    
}

void Object::setMass( const float mass ) {
  this->m_mass = mass;
}

Vec2 Object::getPosition() const {
  return Vec2 ( m_position_x, m_position_y );
}

Vec2 Object::getVelocity() const {
  return Vec2 ( m_velocity_x, m_velocity_y );
}

Vec2 Object::getCenter() const {
  return Vec2 { 0.f, 0.f}; 
}

Vec2 Object::setVelocity( const float vel_x, const float vel_y ) {
  this->m_velocity_x = vel_x; 
  this->m_velocity_y = vel_y; 
  return Vec2 ( m_velocity_x, m_velocity_y );
}

void Object::setVelocity( const Vec2 vel ) {
  m_velocity_x = vel.x;
  m_velocity_y = vel.y;
}

void Object::setAcceleration( const Vec2 acc ) {
  m_acceleration_x = acc.x;
  m_acceleration_y = acc.y;
}

Vec2 Object::getAcceleration() const {
  return Vec2 ( m_acceleration_x, m_acceleration_y );
}

Vec2 Object::setAcceleration( const float acc_x, const float acc_y ) {
  this->m_acceleration_x = acc_x;
  this->m_acceleration_y = acc_y;
  return Vec2 ( m_acceleration_x, m_acceleration_y );
}

void Object::setID( int id ) {
  this->m_object_id = id;
}

Vec2 Object::getSize() const {
  return Vec2 ( 0.0f, 0.0f );
}

int Object::getID(){
  return this->m_object_id;
}

void Object::setQueryBox( AbstractBox<float> refbox ) {
  this->m_queryBox = refbox;
}

AbstractBox<float> Object::getQueryBox() const {
  return this->m_queryBox;
}

bool Object::mouseOnObject( const Vec2& vector ) {
  return false;
}

Object::ObjectType Object::getType(){
  return Object::NONE;
}