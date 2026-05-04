#include <Texture.hpp>

float getRandomNumber(const float& lowerbound, const float upperbound)
{
    static std::mt19937                   gen{std::random_device{}()};
    static std::uniform_real_distribution<float> dist{lowerbound, upperbound};
    return dist(gen);
}

sf::Image createRadialGlowTexture( 
  const float& CORE_RADIUS,
  const float& HALO_TO_CORE_RATIO, 
  const float& FALL_EXP,
  sf::Color* color
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

  syxd::Color new_colors[color_count] = {
    {st_cast_ui8(255), st_cast_ui8(0),   st_cast_ui8(0)},     // Red
    {st_cast_ui8(255), st_cast_ui8(102), st_cast_ui8(102)},   // Light Red
    {st_cast_ui8(255), st_cast_ui8(165), st_cast_ui8(0)},     // Orange
    {st_cast_ui8(255), st_cast_ui8(200), st_cast_ui8(100)},   // Light Orange
    {st_cast_ui8(255), st_cast_ui8(255), st_cast_ui8(255)},   // White
    {st_cast_ui8(255), st_cast_ui8(255), st_cast_ui8(0)},     // Yellow
    {st_cast_ui8(255), st_cast_ui8(255), st_cast_ui8(153)}    // Light Yellow
  };
  
  int rand_color = (int)getRandomNumber(0,color_count)-1;

  sf::Color base;

  if (color) {
      base = *color;
      
  } else {
      base = sf::Color(
          new_colors[rand_color].r,
          new_colors[rand_color].g,
          new_colors[rand_color].b
      );
  }

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



        sf::Uint8 red   = static_cast<sf::Uint8>(base.r * intensity);
        sf::Uint8 green = static_cast<sf::Uint8>(base.g * intensity);
        sf::Uint8 blue  = static_cast<sf::Uint8>(base.b * intensity);
        sf::Uint8 alpha = static_cast<sf::Uint8>(255 * intensity);



        grad.setPixel(x, y, sf::Color(red, green, blue, alpha));
      }
    }
  }
  return grad;
};
