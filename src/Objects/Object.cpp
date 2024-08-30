#include <Object.hpp>

//

const std::map<std::string, Object::ObjectType> Object::m_object_type_map = { 
      { "circle", CIRCLE }, 
      { "rectangle", RECTANGLE }, 
      { "", NONE }, 
      { "cir", CIRCLE },
      { "rec", RECTANGLE }
  };

Object::Object( float mass, float pos_x, float pos_y ) noexcept
: m_mass( mass ), position_current(pos_x, pos_y), position_old(pos_x, pos_y)
{
  this->setVelocity( Vec2 { 0.f, 0.f } );
}

void Object::print_info( ) {
  std::cout << "Mass: " << m_mass << "\nPosition: (" << position_current.x << ", " << position_current.y << ")\n";
  std::cout << "Velocity: (" << velocity.x << ", " << velocity.y << ")\n"; 
 
}

float Object::getMass( ) {
  return m_mass;
} 

std::shared_ptr<sf::Shape> Object::getShape( ) {
  return m_shape;
}

void Object::setOldPosition( ) {
  position_old = position_current;   
}

void Object::setPosition( const Vec2 pos ) {
  position_current = pos;   
}

void Object::setMass( const float mass ) {
  this->m_mass = mass;
}

Vec2 Object::getPosition() const {
  return position_current;
}

Vec2 Object::getOldPosition() const {
  return position_old;
}

Vec2 Object::getVelocity() const {
  return velocity;
}

Vec2 Object::getCenter() const {
  return Vec2 { 0.f, 0.f}; 
}

void Object::setVelocity( const Vec2 vel ) {
  velocity = vel;
}

void Object::setAcceleration( const Vec2 acc ) {
  acceleration = acc;
}

Vec2 Object::getAcceleration() const {
  return acceleration;
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

void Object::EulerIntegration( const float& delta_time ) {
  if ( velocity.magnitude() < 0.01f ) 
    velocity = Vec2( 0.0f, 0.0f );
  
  setVelocity( Vec2( velocity.x + acceleration.x * delta_time , 
                    velocity.y + acceleration.y * delta_time));
  
  setOldPosition();
  setPosition( position_old + velocity * delta_time );
  setAcceleration( Vec2{ 0, 0 });
  setQueryBox( AbstractBox<float>( getCenter()-(getSize()*2), Vec2{ getSize().x*4, getSize().y*4 } ) );
}

void Object::Acceleration( const Vec2& acc ) {
  setAcceleration( acceleration + acc );
}

void Object::applyForce( std::shared_ptr<Object> object, const Vec2& dir, const float& distance ){ 
  // m * m * G / d^2 * dir
  double force_magnitude = (m_mass * object->getMass() * MY_G_CONSTANT ) / (distance * distance);  
  Vec2 force { force_magnitude * -1.0f * dir.x, force_magnitude * -1.0f * dir.y  };
  object->Acceleration( force/object->getMass() );
}

void Object::applyForce( const Vec2& force ){
  Vec2 acc = force / m_mass;
  this->Acceleration(acc);
}

Vec2 Object::getNetForce( ) const {
  return net_force;
}

void Object::AddForce( const Vec2 force ){
  net_force += force;
}