#include "Circle.hpp"
using namespace syxd;

float getRandomNumber(const float& lowerbound, const float upperbound)
{
    static std::mt19937                   gen{std::random_device{}()};
    static std::uniform_real_distribution<float> dist{lowerbound, upperbound};
    return dist(gen);
}

sf::Image createTexture( 
  const float& CORE_RADIUS,
  const float& HALO_TO_CORE_RATIO, 
  const float& FALL_EXP
  ){

  float   R_halo_px = CORE_RADIUS * HALO_TO_CORE_RATIO;
  float   R_full_px = CORE_RADIUS + R_halo_px;
  unsigned texSize  = static_cast<unsigned>(std::ceil(R_full_px * 2.0f));
  float   coreRatio = CORE_RADIUS / R_full_px;

  // 2) Bake CPU gradient into an sf::Image
  sf::Image grad;
  grad.create(texSize, texSize, sf::Color::Transparent);

  float center    = texSize * 0.5f;
  float maxRadius = center;
  
  int rand_color = (int)getRandomNumber(0,7);
  syxd::Color new_colors[color_count] = {
    {st_cast_ui8(255), st_cast_ui8(0),   st_cast_ui8(0)},     // Red
    {st_cast_ui8(255), st_cast_ui8(102), st_cast_ui8(102)},   // Light Red
    {st_cast_ui8(255), st_cast_ui8(165), st_cast_ui8(0)},     // Orange
    {st_cast_ui8(255), st_cast_ui8(200), st_cast_ui8(100)},   // Light Orange
    {st_cast_ui8(255), st_cast_ui8(255), st_cast_ui8(255)},   // White
    {st_cast_ui8(255), st_cast_ui8(255), st_cast_ui8(0)},     // Yellow
    {st_cast_ui8(255), st_cast_ui8(255), st_cast_ui8(153)}    // Light Yellow
  };

  for (unsigned y = 0; y < texSize; ++y)
  {
    for (unsigned x = 0; x < texSize; ++x)
    {
      float dx   = (x + 0.5f) - center;
      float dy   = (y + 0.5f) - center;
      float dist = std::sqrt(dx*dx + dy*dy) / maxRadius; // 0..1

      if (dist <= 1.0f)
      {
        float intensity = (dist <= coreRatio)
            ? 1.0f
            : std::pow(
                1.0f - (dist - coreRatio) / (1.0f - coreRatio),
                FALL_EXP
              );
        sf::Uint8 red   = static_cast<sf::Uint8>(new_colors[rand_color].r * intensity);
        sf::Uint8 green = static_cast<sf::Uint8>(new_colors[rand_color].g * intensity);
        sf::Uint8 blue  = static_cast<sf::Uint8>(new_colors[rand_color].b * intensity);
        sf::Uint8 alpha = static_cast<sf::Uint8>(255 * intensity);



        grad.setPixel(x, y, sf::Color(red, green, blue, alpha));
      }
    }
  }
  return grad;
};

Circle::Circle( float rad, float mass, 
                float posX, float posY,
                bool glow ) noexcept 
                : Object (mass, posX, posY, glow), m_radius(rad) 
{
    if (glow)
    {
      std::cout << "glow mode\n"; 
        // 1. Parameters for core/halo
      const float HALO_TO_CORE = 1.5f;   // halo thickness
      const float FALL_EXP     = 2.0f;   // falloff exponent
      const float CORE_RADIUS = rad;
      float R_halo_px = CORE_RADIUS * HALO_TO_CORE;
      float R_full_px = CORE_RADIUS + R_halo_px;
      unsigned texSize  = static_cast<unsigned>(std::ceil(R_full_px * 2.0f));
      sf::Image grad = createTexture( rad, HALO_TO_CORE, FALL_EXP );
      float center    = texSize * 0.5f;

      // 3. Upload to member texture & enable smoothing
      m_glowTexture.create(texSize, texSize);
      m_glowTexture.update(grad);
      m_glowTexture.setSmooth(true);

      // 4. Create sprite and center its origin
      m_sprite = std::make_shared<sf::Sprite>(m_glowTexture);
      m_sprite->setOrigin(center, center);
      m_sprite->setPosition(posX, posY);

      m_shape = std::make_shared<sf::CircleShape>(rad, 100);
      m_shape->setOrigin(center, center);
      m_shape->setPosition(posX, posY);
    }
    else
    {
      std::cout << "regular shape\n"; 
      
      m_shape = std::make_shared<sf::CircleShape>( m_radius );
      sf::Vector2f circleCenter( rad, rad );
      m_color = std::make_unique<sf::Color>( sf::Color{ static_cast<uint8_t>(rand() % (uint8_t) 255), static_cast<uint8_t>(rand() % (uint8_t) 255), static_cast<uint8_t>(rand() % (uint8_t) 255), 255 } );
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
  std::ostringstream o;
  // keep dim_x/dim_y empty for circle; place rad in its column
  o << id << "," << typeName() << ","
    << m_mass << "," << position_current.x << "," << position_current.y << ","
    << "" << "," << "" << "," << (m_glow ? "1" : "0") << ","; // trailing comma for rad handled below
  // We will instead follow the agreed column order: id,type,mass,pos_x,pos_y,dim_x,dim_y,rad,glow;
  // So rebuild correctly:
  std::ostringstream r;
  r << id << "," << typeName() << ","
    << m_mass << "," << position_current.x << "," << position_current.y << ","
    << "" << "," << "" << "," << m_radius << "," << (m_glow ? "1" : "0") << ";";
  return r.str();
}