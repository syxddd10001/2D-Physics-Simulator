#include "Circle.hpp"


Circle::Circle( float rad, float mass, float posX, float posY)
: Object( mass, posX, posY ), radius( rad )
{ 
    shape = new sf::CircleShape( radius );
    sf::Vector2f circleCenter( rad, rad );
    
    color = new sf::Color(sf::Color::White);
    shape->setOrigin( circleCenter );
    shape->setOutlineThickness( 1.0f );
    shape->setFillColor( *color );
    shape->setOutlineColor( sf::Color::Black );
    setQueryBox( AbstractBox<float>( getCenter()-(getSize()*2) ,  getSize()*4 )) ;
    

}
Circle::Circle( ){

}

sf::Shape* Circle::getShape( ){
    return this->shape;
}

Vec2 Circle::getSize( ){
  return Vec2( radius, radius );
}

float Circle::setRadius( const float rad ){
    this->radius = rad;
    return radius;
}

float Circle::getRadius( ){
    return radius;
}

void Circle::setPosition( const Vec2 pos ){
    position_x = pos.x;
    position_y = pos.y;
    shape->setPosition( sf::Vector2f( position_x, position_y ) );
    
}

Vec2 Circle::getCenter(){
  return Vec2{ position_x, position_y };
}

bool Circle::mouseOnObject( const sf::Vector2f vector ){
  return calculateDistance( getPosition(), Vec2 ( vector.x, vector.y ) ) < getRadius( );
}

AbstractBox<float> Circle::getBoundingBox( ) {
  float left = this->position_x - this->radius;
  float top = this->position_y - this->radius;
  float width = this->radius * 2;
  float height = this->radius * 2;
  return AbstractBox<float>(left, top, width, height);
}