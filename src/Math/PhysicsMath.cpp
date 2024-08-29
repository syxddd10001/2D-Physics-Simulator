#include "PhysicsMath.hpp"

#define DEBUG 1
#if DEBUG == 1
#define DEBUG_PRINT(format, ...) printf(format, ##__VA_ARGS__)
#else
#define DEBUG_PRINT(format, ...) // Do nothing //
#endif


point pairAdd( const point& pair1, const point& pair2 ) {
  return point( pair1.first+pair2.first, pair1.second+pair2.second );
}    

point pairSubtract( const point& pair1, const point& pair2 ) {
  return point( pair1.first-pair2.first, pair1.second-pair2.second );
}

point pairMultiply( const point& pair1, const point& pair2 ) {
  return point( pair1.first*pair2.first, pair1.second*pair2.second );
}

float calculateDistance( const Vec2& pos1, const Vec2& pos2 ) {
  Vec2 difference = pos1 - pos2;
  return std::sqrt( difference.x * difference.x + difference.y * difference.y );
}

float dotProduct( const point& vectorA, const point& vectorB ) {
  return vectorA.first*vectorB.first + vectorA.second*vectorB.second;
}

void verletIntegration( std::shared_ptr<Object> object, const float delta_time, const Vec2 acceleration ) {
  Vec2 velocity_0 = object->getVelocity();
  Vec2 position_0 = object->getPosition();

  if ( std::sqrt( velocity_0.x*velocity_0.x + velocity_0.y * velocity_0.y ) < 0.01f ) 
    velocity_0 = Vec2( 0.0f, 0.0f );
  
  Vec2 acc = object->getAcceleration();
  
  object->setVelocity( Vec2( velocity_0.x + ( velocity_0.x * acceleration.x) * delta_time , 
                              velocity_0.y + ( velocity_0.y * +acceleration.y) * delta_time));
  std::cout << object->getVelocity().x << ',' << object->getVelocity().y <<'\n';
  object->setPosition( Vec2( position_0.x + (object->getVelocity().x * delta_time ), 
                              position_0.y + (object->getVelocity().y * delta_time ) ) );

  object->setQueryBox( AbstractBox<float>( object->getCenter()-(object->getSize()*2), Vec2{ object->getSize().x*4, object->getSize().y*4 } ) );

}

Vec2 applyForce( std::shared_ptr<Object> object ) {
  return Vec2{};
}

Vec2 normalizeVector( const Vec2 p ) {
  float magnitude = sqrt( p.x * p.x + p.y * p.y );
  return ( (magnitude != 0) ? Vec2( p.x / magnitude, p.y / magnitude ) : Vec2( 0, 0 ) );
}

Vec2 applyForce( std::shared_ptr<Object> this_object, std::shared_ptr<Object>other_object, const float delta_time ) {
  float distance = calculateDistance( this_object->getPosition( ), other_object->getPosition( ) );
  float force_mag = ( ( GRAVITATIONAL_CONSTANT * this_object->getMass( ) * other_object->getMass( ) ) / ( distance * distance ) );
  Vec2 force_direction = normalizeVector( other_object->getPosition( ) - this_object->getPosition( ) );
  Vec2 force ( force_direction.x * force_mag / other_object->getMass(), force_direction.y * force_mag / other_object->getMass());

  std::cout << force.x << ", " << force.y << '\n';
  return force;
}

std::pair<float, Vec2> calculateCenterOfMass( const std::vector<std::shared_ptr<Object>> objects ) {
  assert( objects.size() != 0 );
  float total_mass = 0.0f;
  float x = 0.0f;
  float y = 0.0f;
  for ( const auto& p : objects ){
    float mass = p->getMass();
    Vec2 pos = p->getCenter();
    x += mass * pos.x;
    y += mass * pos.y;
    total_mass += mass;
  }
  assert ( total_mass > 0.0f );
  
  return std::make_pair( total_mass , Vec2 { x/total_mass, y/total_mass } );
  
}