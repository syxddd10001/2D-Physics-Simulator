#include "Physics.hpp"


bool onCollision( Circle* object1, Circle* object2 ) {
    
  float distance = calculateDistance( object1->getPosition(), object2->getPosition() );
  float push_distance;
  float radius1 = object1->getRadius();
  float radius2 = object2->getRadius();

  point delta ( 
    object1->getPosition().first - object2->getPosition().first, 
    object1->getPosition().second - object2->getPosition().second 
  );

  if ( distance < ( radius1 + radius2 ) ){
    push_distance = 1.0f * ( distance - radius1 - radius2 ); 
    point position1 ( 
      object1->getPosition().first - ( push_distance * delta.first/distance ), 
      object1->getPosition().second - ( push_distance * delta.second/distance ) 
    );
    point position2 ( 
      object2->getPosition().first + ( push_distance * delta.first/distance ), 
      object2->getPosition().second + ( push_distance * delta.second/distance ) 
    );
      
    object1->setPosition( position1 );
    object2->setPosition( position2 );
    return true;
  }

  return false;
}

void dynamicResponse( Circle* object1, Circle* object2 ) {
  point position1 ( object1->getPosition( ) );
  point position2 ( object2->getPosition( ) );
  point velocity1 ( object1->getVelocity( ) );
  point velocity2 ( object2->getVelocity( ) );
  float mass1 = object1->getMass( );
  float mass2 = object2->getMass( );
  
  point collision_normal ( pairSubtract( position1, position2 ) );

  float distance = calculateDistance( position1, position2 );

  if ( distance != 0.0f ) collision_normal = std::make_pair( collision_normal.first / distance, collision_normal.second / distance );

  point collision_tangent( -collision_normal.second, collision_normal.first );

  float dot_velocity_tangent1 = dotProduct( velocity1, collision_tangent ); 
  float dot_velocity_tangent2 = dotProduct( velocity2, collision_tangent );

  float dot_velocity_normal1 = dotProduct( velocity1, collision_normal );
  float dot_velocity_normal2 = dotProduct( velocity2, collision_normal );

  float momentum1 = ( dot_velocity_normal1 * ( mass1 - mass2 ) + 2.0f * mass2 * dot_velocity_normal2 ) / ( mass1 + mass2 );
  float momentum2 = ( dot_velocity_normal2 * ( mass2 - mass1 ) + 2.0f * mass1 * dot_velocity_normal1 ) / ( mass1 + mass2 ); 

  velocity1 = pairAdd( 
                point( collision_tangent.first * dot_velocity_tangent1, collision_tangent.second * dot_velocity_tangent1 ),  
                point( collision_normal.first * momentum1, collision_normal.second * momentum1 )
                );
  
  velocity2 = pairAdd( 
                point( collision_tangent.first * dot_velocity_tangent2, collision_tangent.second * dot_velocity_tangent2 ),  
                point( collision_normal.first * momentum2, collision_normal.second * momentum2 )
                );
  
  object1->setVelocity(velocity1);
  object2->setVelocity(velocity2);  
}