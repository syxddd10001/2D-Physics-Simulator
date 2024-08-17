#include "Rectangle.hpp"

Rectangle::Rectangle( float mass, float pos_x, float pos_y, float dimX, float dimY ) noexcept
: Object( mass, pos_x, pos_y ), m_dimensions( dimX, dimY )  {
  m_shape = std::make_shared<sf::RectangleShape>(sf::Vector2f(dimX, dimY));
  m_color = std::make_shared<sf::Color>( sf::Color{ rand() % 256, rand() % 256, rand() % 256, 255 } );
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
  return Vec2{ m_position_x+(m_dimensions.x/2), m_position_y+(m_dimensions.y/2) };
}

void Rectangle::setPosition( const Vec2 pos ) {
  m_position_x = pos.x;
  m_position_y = pos.y;
  m_shape->setPosition( sf::Vector2f( pos.x, pos.y ) );
}

bool Rectangle::inBounds( const sf::Vector2f mousepos ) {   
  return ( ( mousepos.x >= m_position_x && mousepos.x <= m_position_x + m_dimensions.x ) &&
          ( mousepos.y >= m_position_y && mousepos.y <= m_position_y + m_dimensions.y ) );
}

bool Rectangle::mouseOnObject( const Vec2& vector ) {
  return inBounds( vector );
}

AbstractBox<float> Rectangle::getBoundingBox( ) const {
  float left = this->m_position_x;
  float top = this->m_position_y;
  float width = m_dimensions.x;
  float height = m_dimensions.y;
  return AbstractBox<float>( left, top, width, height );
}


bool Rectangle::intersects( std::shared_ptr<Rectangle> other ) const noexcept {
  Vec2 other_dim = other->getDimension();
  return ( m_position_x < other->m_position_x + other_dim.x &&
          m_position_x + m_dimensions.x > other->m_position_x &&
           m_position_y < other->m_position_y + other_dim.y &&
          m_position_y + m_dimensions.y > other->m_position_y);
}