#include "Rectangle.hpp"

Rectangle::Rectangle( float mass, float pos_x, float pos_y, float dimX, float dimY )
: Object( mass, pos_x, pos_y ), dimensions( dimX, dimY )
{
  shape = std::make_shared<sf::RectangleShape>(sf::Vector2f(dimX, dimY));
  color = std::make_shared<sf::Color>(sf::Color::White);
  shape->setOutlineThickness(1.0f);
  shape->setOutlineColor(sf::Color::Black);
  shape->setFillColor(*color);
  setQueryBox( AbstractBox<float>( getCenter()-(getSize()*2) , getSize()*4 ) ) ;
  
}

Rectangle::Rectangle( ){

}

Vec2 Rectangle::getSize( ) const {
  return Vec2( dimensions.x, dimensions.y );
}

Vec2 Rectangle::getDimension( ){
  return Vec2( this->dimensions.x, this->dimensions.y );
}

void Rectangle::setDimension( const Vec2 dim ){
  this->dimensions.x = dim.x;
  this->dimensions.y = dim.y;
}

std::shared_ptr<sf::Shape> Rectangle::getShape(){
  return this->shape;
}

Vec2 Rectangle::getCenter( ) const {
  return Vec2{ position_x+(dimensions.x/2), position_y+(dimensions.y/2) };
}

void Rectangle::setPosition( const Vec2 pos ){
  position_x = pos.x;
  position_y = pos.y;
  shape->setPosition( sf::Vector2f( pos.x, pos.y ) );
}

bool Rectangle::inBounds( const sf::Vector2f mousepos ) {   
  return ( ( mousepos.x >= position_x && mousepos.x <= position_x + dimensions.x ) &&
          ( mousepos.y >= position_y && mousepos.y <= position_y + dimensions.y ) );
}

bool Rectangle::mouseOnObject( const Vec2 vector ){
  return inBounds( vector );
}

AbstractBox<float> Rectangle::getBoundingBox() const {
  float left = this->position_x;
  float top = this->position_y;
  float width = dimensions.x;
  float height = dimensions.y;
  return AbstractBox<float>(left, top, width, height);
}