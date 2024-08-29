#include "Rectangle.hpp"

Rectangle::Rectangle( float mass, float pos_x, float pos_y, float dimX, float dimY ) noexcept
: Object( mass, pos_x, pos_y ), m_dimensions( dimX, dimY )  {
  m_shape = std::make_shared<sf::RectangleShape>(sf::Vector2f(dimX, dimY));
  m_color = std::make_shared<sf::Color>( sf::Color{ static_cast<uint8_t>(rand() % (uint8_t) 255), static_cast<uint8_t>(rand() % (uint8_t) 255), static_cast<uint8_t>(rand() % (uint8_t) 255), 255 } );
  m_shape->setOutlineThickness(1.0f);
  m_shape->setOutlineColor(sf::Color::Black);
  m_shape->setFillColor(*m_color);
  setQueryBox( AbstractBox<float>( getCenter()-(getSize()*2) , getSize()*4 ) ) ; 
}

Rectangle::Rectangle( ) { }

Vec2 Rectangle::getSize( ) const {
  return Vec2( m_dimensions.x, m_dimensions.y );
}

Vec2 Rectangle::getDimension( ) {
  return Vec2( this->m_dimensions.x, this->m_dimensions.y );
}

void Rectangle::setDimension( const Vec2 dim ) {
  this->m_dimensions.x = dim.x;
  this->m_dimensions.y = dim.y;
}

std::shared_ptr<sf::Shape> Rectangle::getShape() {
  return this->m_shape;
}

Vec2 Rectangle::getCenter( ) const {
  return Vec2{ position_current.x+(m_dimensions.x/2), position_current.y+(m_dimensions.y/2) };
}

void Rectangle::setPosition( const Vec2 pos ) {
  position_current = pos;
  m_shape->setPosition( sf::Vector2f( pos.x, pos.y ) );
}

bool Rectangle::inBounds( const sf::Vector2f mousepos ) {   
  return ( ( mousepos.x >= position_current.x && mousepos.x <= position_current.x + m_dimensions.x ) &&
          ( mousepos.y >= position_current.y && mousepos.y <= position_current.y + m_dimensions.y ) );
}

bool Rectangle::mouseOnObject( const Vec2& vector ) {
  return inBounds( vector );
}

AbstractBox<float> Rectangle::getBoundingBox( ) const {
  float left = this->position_current.x;
  float top = this->position_current.y;
  float width = m_dimensions.x;
  float height = m_dimensions.y;
  return AbstractBox<float>( left, top, width, height );
}

bool Rectangle::intersects( std::shared_ptr<Rectangle> other ) const noexcept {
  Vec2 other_dim = other->getDimension();
  return ( position_current.x < other->position_current.x + other_dim.x &&
          position_current.x+ m_dimensions.x > other->position_current.x &&
          position_current.y < other->position_current.y + other_dim.y &&
          position_current.y + m_dimensions.y > other->position_current.y );
}