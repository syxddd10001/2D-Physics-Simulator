#include "Circle.hpp"
using namespace syxd;

Circle::Circle( float rad, float mass, float posX, float posY) noexcept
: Object( mass, posX, posY ), m_radius( rad ) { 
  m_shape = std::make_shared<sf::CircleShape>( m_radius );
  sf::Vector2f circleCenter( rad, rad );
  m_color = std::make_shared<sf::Color>( sf::Color{ static_cast<uint8_t>(rand() % (uint8_t) 255), static_cast<uint8_t>(rand() % (uint8_t) 255), static_cast<uint8_t>(rand() % (uint8_t) 255), 255 } );
  m_shape->setOrigin( circleCenter );
  m_shape->setOutlineThickness( 0.5f );
  m_shape->setFillColor( *m_color );
  m_shape->setOutlineColor( sf::Color::Black );
  setQueryBox( AbstractBox<float>( getCenter()-(getSize()*2) ,  getSize()*4 )) ;
}
Circle::Circle( ) { }

/*
Returns a pointer to the shape of THIS object
*/
std::shared_ptr<sf::Shape> Circle::getShape( ) {
    return this->m_shape;
}

Vec2 Circle::getSize( ) const {
  return Vec2( m_radius, m_radius );
}

float Circle::setRadius( const float rad ) {
    this->m_radius = rad;
    return m_radius;
}

float Circle::getRadius( ) {
    return m_radius;
}

void Circle::setPosition( const Vec2 pos ) {
    position_current = pos;
    m_shape->setPosition( sf::Vector2f( position_current.x, position_current.y ) );
}

Vec2 Circle::getCenter() const {
  return position_current;
}

bool Circle::mouseOnObject( const Vec2& vector ) {
  return calculateDistance( getPosition(), vector ) < getRadius( );
}

AbstractBox<float> Circle::getBoundingBox( ) const {
  float left = this->position_current.x - this->m_radius;
  float top = this->position_current.y - this->m_radius;
  float width = this->m_radius * 2;
  float height = this->m_radius * 2;
  return AbstractBox<float>(left, top, width, height);
}
