#include "Physics.hpp"

// Detect circle vs. circle collisions
bool onCollision( std::shared_ptr<Circle> object1, std::shared_ptr<Circle> object2 ) {
    
  float distance = calculateDistance( object1->getPosition(), object2->getPosition() );
  float push_distance;
  float radius1 = object1->getRadius();
  float radius2 = object2->getRadius();

  Vec2 delta ( 
    object1->getPosition().x - object2->getPosition().x, 
    object1->getPosition().y - object2->getPosition().y 
  );

  if ( distance < ( radius1 + radius2 ) ) {
    push_distance = 1.0f * ( distance - radius1 - radius2 ); 
    Vec2 position1 ( 
      object1->getPosition().x - ( push_distance * delta.x/distance ), 
      object1->getPosition().y - ( push_distance * delta.y/distance ) 
    );
    Vec2 position2 ( 
      object2->getPosition().x + ( push_distance * delta.x/distance ), 
      object2->getPosition().y + ( push_distance * delta.y/distance ) 
    );
      
    object1->setPosition( position1 );
    object2->setPosition( position2 );
    return true;
  }

  return false;
}

// Response to circle vs. circle collisions
void dynamicResponse( std::shared_ptr<Circle> object1, std::shared_ptr<Circle> object2 ) {
  Vec2 position1 ( object1->getPosition( ) );
  Vec2 position2 ( object2->getPosition( ) );
  Vec2 velocity1 ( object1->getVelocity( ) );
  Vec2 velocity2 ( object2->getVelocity( ) );
  float mass1 = object1->getMass( );
  float mass2 = object2->getMass( );
  
  Vec2 collision_normal ( position1 - position2 );

  float distance = calculateDistance( position1, position2 );

  if ( distance != 0.0f ) collision_normal = Vec2( collision_normal.x / distance, collision_normal.y / distance );

  Vec2 collision_tangent( -collision_normal.y, collision_normal.x );

  float dot_velocity_tangent1 = velocity1 * collision_tangent ; 
  float dot_velocity_tangent2 = velocity2 * collision_tangent ;

  float dot_velocity_normal1 = velocity1 * collision_normal ;
  float dot_velocity_normal2 = velocity2 * collision_normal ;

  float momentum1 = ( dot_velocity_normal1 * ( mass1 - mass2 ) + 2.0f * mass2 * dot_velocity_normal2 ) / ( mass1 + mass2 );
  float momentum2 = ( dot_velocity_normal2 * ( mass2 - mass1 ) + 2.0f * mass1 * dot_velocity_normal1 ) / ( mass1 + mass2 ); 

  velocity1 = Vec2( collision_tangent.x * dot_velocity_tangent1, collision_tangent.y * dot_velocity_tangent1 ) +  
                Vec2( collision_normal.x * momentum1, collision_normal.y * momentum1 );
  
  velocity2 =  Vec2( collision_tangent.x * dot_velocity_tangent2, collision_tangent.y * dot_velocity_tangent2 ) +   
                Vec2( collision_normal.x * momentum2, collision_normal.y * momentum2 ) ;
  
  object1->setVelocity(velocity1);
  object2->setVelocity(velocity2);  
}

// Detect Circle v Rectangle collision
bool onCollision( std::shared_ptr<Circle> object1, std::shared_ptr<Rectangle> object2 ) {
  Vec2 nearestPoint;
  Vec2 circ_pos = object1->getPosition();
  Vec2 rect_pos = object2->getPosition();
  nearestPoint.x = std::max( rect_pos.x, std::min( circ_pos.x, rect_pos.x+object2->getDimension().x ) );
  nearestPoint.y = std::max( rect_pos.y, std::min( circ_pos.y, rect_pos.y+object2->getDimension().y ) );
  Vec2 RayToNearest = nearestPoint - circ_pos;
  float overlap = object1->getRadius() - RayToNearest.magnitude();
  
  if ( std::isnan( overlap ) ) overlap = 0;
  
  if (overlap > 0){
    object1->setPosition( circ_pos - RayToNearest.normalize() * overlap );
    return true;
  } 
  
  return false;
}
// Response for Circle v Rectangle collision response
void dynamicResponse( std::shared_ptr<Circle> object1, std::shared_ptr<Rectangle> object2 ) {
  
}