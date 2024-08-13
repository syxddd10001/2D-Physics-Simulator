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

Vec2 verletIntegration( std::shared_ptr<Object> object, const float delta_time, const Vec2 acceleration ) {
  Vec2 velocity_0 = object->getVelocity();
  Vec2 position_0 = object->getPosition();

  if ( std::sqrt( velocity_0.x*velocity_0.x + velocity_0.y * velocity_0.y ) < 0.01f ) 
  velocity_0 = Vec2( 0.0f, 0.0f );
  
  
  object->setAcceleration( calculateAcceleration( object, delta_time, velocity_0 ) );
  Vec2 acc = object->getAcceleration();
  
  object->setVelocity( Vec2( velocity_0.x + ( velocity_0.x * acceleration.x) * delta_time , 
                              velocity_0.y + ( velocity_0.y * +acceleration.y) * delta_time));

  
  object->setPosition( Vec2( position_0.x + (object->getVelocity().x * delta_time ), 
                              position_0.y + (object->getVelocity().y * delta_time ) ) );

  object->setQueryBox( AbstractBox<float>( object->getCenter()-(object->getSize()*2), Vec2{ object->getSize().x*4, object->getSize().y*4 } ) );
  
  return object->getPosition();
}

Vec2 applyForce( std::shared_ptr<Object> object ) {
  
  
  return Vec2{};

}

Vec2 calculateAcceleration( std::shared_ptr<Object> object, const float delta_time, const Vec2 velocity ) {
  Vec2 acceleration = velocity - object->getVelocity();
  acceleration = Vec2( acceleration.x / delta_time, acceleration.y / delta_time );
  return Vec2( acceleration.x, acceleration.y ); // Assuming equal acceleration in both directions
}

Vec2 normalizeVector( const Vec2 p ) {
  float magnitude = sqrt( p.x * p.x + p.y * p.y );
  return ( (magnitude != 0) ? Vec2( p.x / magnitude, p.y / magnitude ) : Vec2( 0, 0 ) );
}

Vec2 applyForce( std::shared_ptr<Object> this_object, std::shared_ptr<Object>other_object, const float delta_time ){
  float distance = calculateDistance( this_object->getPosition( ), other_object->getPosition( ) );
  float force_mag = ( ( GRAVITATIONAL_CONSTANT * this_object->getMass( ) * other_object->getMass( ) ) / ( distance * distance ) );
  Vec2 forceDirection = normalizeVector( other_object->getPosition( ) - this_object->getPosition( ) );
  Vec2 force ( forceDirection.x * force_mag / other_object->getMass(), forceDirection.y * force_mag / other_object->getMass());

  std::cout << force.x << ", " << force.y << '\n';
  return force;
}

