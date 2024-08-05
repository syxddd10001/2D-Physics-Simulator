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

float calculateDistance( point pos1, point pos2 ) {
  point difference = pairSubtract( pos1, pos2 );
  return std::sqrt( difference.first * difference.first + difference.second * difference.second );
}

float dotProduct( point vectorA, point vectorB ) {
  return vectorA.first*vectorB.first + vectorA.second*vectorB.second;
}

point calculateVelocity( Object* object, const float delta_time, const point acceleration ){
  point velocity_0 = object->getVelocity();
  point position_0 = object->getPosition();

  if ( std::sqrt( velocity_0.first*velocity_0.first + velocity_0.second * velocity_0.second ) < 0.01f ) 
  velocity_0 = std::make_pair( 0.0f, 0.0f );
  
  object->setVelocity( point( velocity_0.first + ( velocity_0.first* (acceleration.first) * delta_time ), 
                              velocity_0.second+ ( velocity_0.second* (acceleration.second) * delta_time ) ) );
  
  object->setAcceleration( calculateAcceleration( object, delta_time, velocity_0 ) );
  
  object->setPosition( point( position_0.first + (object->getVelocity().first * delta_time ), 
                              position_0.second + (object->getVelocity().second * delta_time ) ) );

  return object->getPosition();
}
//
point calculateVelocity( Object* object, float delta_time ) {
  point velocity_0 = object->getVelocity();
  point position_0 = object->getPosition();

  if ( std::sqrt( velocity_0.first*velocity_0.first + velocity_0.second * velocity_0.second ) < 0.01f ) 
  velocity_0 = std::make_pair( 0.0f, 0.0f );
  
  object->setVelocity( point( velocity_0.first + ( velocity_0.first * delta_time ), 
                              velocity_0.second+ ( velocity_0.second * delta_time ) ) );
  
  object->setAcceleration( calculateAcceleration( object, delta_time, velocity_0 ) );
  
  object->setPosition( point( position_0.first + (object->getVelocity().first * delta_time ), 
                              position_0.second + (object->getVelocity().second * delta_time ) ) );

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

point calculateAcceleration( Object* object, float delta_time, point velocity ) {
  point acceleration = pairSubtract( velocity, object->getVelocity() );
  acceleration = point( acceleration.first / delta_time, acceleration.second / delta_time );
  return std::pair<double, double>( acceleration.first, acceleration.second ); // Assuming equal acceleration in both directions
}

point normalizeVector( const point p ) {
  float magnitude = sqrt( p.first * p.first + p.second * p.second );
  return ( (magnitude != 0) ? point( p.first / magnitude, p.second / magnitude ) : point( 0, 0 ) );
}

point applyForce( Object* this_object, Object* other_object, const float delta_time ){
  float distance = calculateDistance( this_object->getPosition( ), other_object->getPosition( ) );
  float force_mag = ( ( GRAVITATIONAL_CONSTANT * this_object->getMass( ) * other_object->getMass( ) ) / ( distance * distance ) );
  point forceDirection = normalizeVector( pairSubtract( other_object->getPosition( ), this_object->getPosition( ) ) );
  point force ( forceDirection.first * force_mag / other_object->getMass(), forceDirection.second * force_mag / other_object->getMass());

  std::cout << force.first << ", " << force.second << '\n';
  return force;
}

