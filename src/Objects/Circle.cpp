#include "Circle.hpp"


Circle::Circle( float rad, float mass, float posX, float posY)
: Object( mass, posX, posY ), radius( rad )
{ 
    shape = std::make_shared<sf::CircleShape>( radius );
    sf::Vector2f circleCenter( rad, rad );
    color = std::make_shared<sf::Color>(sf::Color::White);
    shape->setOrigin( circleCenter );
    shape->setOutlineThickness( 0.5f );
    shape->setFillColor( *color );
    shape->setOutlineColor( sf::Color::Black );
    setQueryBox( AbstractBox<float>( getCenter()-(getSize()*2) ,  getSize()*4 )) ;
    

}
Circle::Circle( ){

}

std::shared_ptr<sf::Shape> Circle::getShape( ){
    return this->shape;
}

Vec2 Circle::getSize( ) const {
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

Vec2 Circle::getCenter() const {
  return Vec2{ position_x, position_y };
}

bool Circle::mouseOnObject( const Vec2& vector ){
  return calculateDistance( getPosition(), vector ) < getRadius( );
}

AbstractBox<float> Circle::getBoundingBox( ) const {
  float left = this->position_x - this->radius;
  float top = this->position_y - this->radius;
  float width = this->radius * 2;
  float height = this->radius * 2;
  return AbstractBox<float>(left, top, width, height);
}