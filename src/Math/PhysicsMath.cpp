#include "PhysicsMath.hpp"

#define DEBUG 1
#if DEBUG == 1
#define DEBUG_PRINT(format, ...) printf(format, ##__VA_ARGS__)
#else
#define DEBUG_PRINT(format, ...) // Do nothing //
#endif


point pairAdd( point pair1, point pair2 ) {
  return point( pair1.first+pair2.first, pair1.second+pair2.second );
}    

point pairSubtract( point pair1, point pair2 ) {
  return point( pair1.first-pair2.first, pair1.second-pair2.second );
}

point pairMultiply( point pair1, point pair2 ) {
  return point( pair1.first*pair2.first, pair1.second*pair2.second );
}

float calculateDistance( Vec2 pos1, Vec2 pos2 ){
  Vec2 difference = pos1 - pos2;
  return std::sqrt( difference.x * difference.x + difference.y * difference.y );
}

float dotProduct( point vectorA, point vectorB ) {
  return vectorA.first*vectorB.first + vectorA.second*vectorB.second;
}

Vec2 calculateVelocity( Object* object, const float delta_time, const Vec2 acceleration ) {
  Vec2 velocity_0 = object->getVelocity();
  Vec2 position_0 = object->getPosition();

  if ( std::sqrt( velocity_0.x*velocity_0.x + velocity_0.y * velocity_0.y ) < 0.01f ) 
  velocity_0 = Vec2( 0.0f, 0.0f );
  
  object->setVelocity( Vec2( velocity_0.x + ( velocity_0.x* (acceleration.x) * delta_time ), 
                              velocity_0.y+ ( velocity_0.y* (acceleration.y) * delta_time ) ) );
  
  object->setAcceleration( calculateAcceleration( object, delta_time, velocity_0 ) );
  
  object->setPosition( Vec2( position_0.x + (object->getVelocity().x * delta_time ), 
                              position_0.y + (object->getVelocity().y * delta_time ) ) );

  return object->getPosition();
}

double applyForce( Object* object1, Object* object2 ) {
  double distance = calculateDistance( object1->getPosition(), object2->getPosition() );
  double mass_1 = object1->getMass();
  double mass_2 = object2->getMass();
  std::cout << std::setprecision( 13 );
  std::cout << "dist:" << distance << std::endl;
  double force =  ( GRAVITATIONAL_CONSTANT * mass_1 * mass_2 ) / ( distance*distance );
  
  return force;

}

Vec2 calculateAcceleration( Object* object, float delta_time, Vec2 velocity ) {
  Vec2 acceleration = velocity - object->getVelocity();
  acceleration = Vec2( acceleration.x / delta_time, acceleration.y / delta_time );
  return Vec2( acceleration.x, acceleration.y ); // Assuming equal acceleration in both directions
}

Vec2 normalizeVector( const Vec2 p ) {
  float magnitude = sqrt( p.x * p.x + p.y * p.y );
  return ( (magnitude != 0) ? Vec2( p.x / magnitude, p.y / magnitude ) : Vec2( 0, 0 ) );
}

Vec2 applyForce( Object* this_object, Object* other_object, const float delta_time ){
  float distance = calculateDistance( this_object->getPosition( ), other_object->getPosition( ) );
  float force_mag = ( ( GRAVITATIONAL_CONSTANT * this_object->getMass( ) * other_object->getMass( ) ) / ( distance * distance ) );
  Vec2 forceDirection = normalizeVector( other_object->getPosition( ) - this_object->getPosition( ) );
  Vec2 force ( forceDirection.x * force_mag / other_object->getMass(), forceDirection.y * force_mag / other_object->getMass());

  std::cout << force.x << ", " << force.y << '\n';
  return force;
}

