#include "Circle.hpp"
using namespace syxd;

Circle::Circle( float rad, float mass, 
                float posX, float posY,
                bool glow, sf::Color* color ) noexcept 
                : Object (mass, posX, posY, glow), m_radius(rad) 
{
  if (glow) {
    if ( color != nullptr ){
      m_color = std::make_unique<sf::Color>( *color );

    } else {
      m_color = std::make_unique<sf::Color>( sf::Color{ static_cast<uint8_t>(rand() % (uint8_t) 255), static_cast<uint8_t>(rand() % (uint8_t) 255), static_cast<uint8_t>(rand() % (uint8_t) 255), 255 } );
      color = m_color.get();
    }
    makeCircleGlow( color );

  } else {
    m_shape = std::make_shared<sf::CircleShape>( m_radius );
    sf::Vector2f circleCenter( rad, rad );

    if ( color != nullptr ){
      m_color = std::make_unique<sf::Color>( *color );

    } else {
      m_color = std::make_unique<sf::Color>( sf::Color{ static_cast<uint8_t>(rand() % (uint8_t) 255), static_cast<uint8_t>(rand() % (uint8_t) 255), static_cast<uint8_t>(rand() % (uint8_t) 255), 255 } );
    }
    m_shape->setOrigin( circleCenter );
    m_shape->setOutlineThickness( 0.5f );
    m_shape->setFillColor( *m_color );
    m_shape->setOutlineColor( *m_color );
    m_glow = false;
    setQueryBox( AbstractBox<float>( getCenter()-(getSize()*2) ,  getSize()*4 )) ;
  }

  // 5) Initialize the query‐box for spatial queries
  setQueryBox(
      AbstractBox<float>(
          getCenter() - (getSize() * 2.0f),
          getSize() * 4.0f
      )
  );
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
  if ( m_shape != nullptr ) m_shape->setPosition( sf::Vector2f( position_current.x, position_current.y ) );
  if ( m_sprite != nullptr ) m_sprite->setPosition( sf::Vector2f( position_current.x, position_current.y ) );

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

void Circle::draw( std::shared_ptr<sf::RenderWindow> WINDOW ){
  if ( m_glow ){
    if ( m_sprite != nullptr ) {
      WINDOW->draw( *m_sprite, sf::BlendAdd );
      return;
    }
  }

  if ( m_shape != nullptr) {
    WINDOW->draw( *m_shape );
  }
}

std::string Circle::serializeCSV(int id) const {
  std::ostringstream r;
  long long int color_int = m_color ? m_color->toInteger() : -1;
  r << id << ","
    << typeName() << ","
    << m_mass << ","
    << position_current.x << ","
    << position_current.y << ","
    << "" << ","              // dim_x (unused for circle)
    << "" << ","              // dim_y (unused for circle)
    << m_radius << ","
    << (m_glow ? "1" : "0") << ","
    << velocity.x << ","
    << velocity.y << ","
    << color_int << ";";
  return r.str();
}

void Circle::makeCircleGlow( sf::Color* color ){
  Vec2 position = getPosition();
  float posX = position.x;
  float posY = position.y;
  
  const float HALO_TO_CORE = 1.5f;   // halo thickness
  const float FALL_EXP     = 2.0f;   // falloff exponent
  const float CORE_RADIUS = m_radius;
  float R_halo_px = CORE_RADIUS * HALO_TO_CORE;
  float R_full_px = CORE_RADIUS + R_halo_px;
  unsigned texSize  = static_cast<unsigned>(std::ceil(R_full_px * 2.0f));
  sf::Image grad = createRadialGlowTexture( m_radius, HALO_TO_CORE, FALL_EXP, color );
  float center    = texSize * 0.5f;

  // 3. Upload to member texture & enable smoothing
  m_glowTexture.create(texSize, texSize);
  m_glowTexture.update(grad);
  m_glowTexture.setSmooth(true);

  // 4. Create sprite and center its origin
  m_sprite = std::make_shared<sf::Sprite>(m_glowTexture);
  m_sprite->setOrigin(center, center);
  m_sprite->setPosition(getPosition().x, getPosition().y);

  m_shape = std::make_shared<sf::CircleShape>(m_radius, 100);
  m_shape->setOrigin(center, center);
  m_shape->setPosition(posX, posY);
}