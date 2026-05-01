#include <Button.hpp>

void Button::setFont( const std::string path ) {
  if ( !m_font.loadFromFile( path ) ){
    std::cout << "Font not found!\n";
    return;
  }
  
  m_text.setFont(m_font);
  
}

void Button::setFontDefault( ) { // THIS IS BAD FIX THIS
  if ( !m_font.loadFromFile( "static/fonts/cairo.ttf" ) ){
    std::cout << "Font not found!\n";
    return;
  }
  std::cout << "font set successfully\n";
}

void Button::setPosition( const sf::Vector2f& position ){
  UI_Element::setPosition(position);
  m_box.setPosition( position );
  m_text.setPosition( (position.x+m_size.x)/2, (position.y+m_size.y)/2);
}

void Button::render(sf::RenderWindow* WINDOW_REF, const float& delta_time ) {
  sf::Vector2i pixel_pos = sf::Mouse::getPosition(*WINDOW_REF);
  sf::Vector2f mouse_pos = WINDOW_REF->mapPixelToCoords(pixel_pos);
  bool hovered = mouseOnObject( Vec2(mouse_pos) );

  if (m_hover) {  
    onHover( mouse_pos, hovered, delta_time );
  }

  if (hovered){
    onClick( hovered );
  }
  
  WINDOW_REF->draw(m_box);
  WINDOW_REF->draw(m_text);
}

void Button::onHover( sf::Vector2f mouse_pos, bool hovered, const float& delta_time ){
  float speed = 10.f;

  // EASING
  if (hovered)
      hoverAmount += speed * delta_time * (1.f - hoverAmount);
  else
      hoverAmount -= speed * delta_time * hoverAmount;

  hoverAmount = hoverAmount < 0.f ? 0.f :
                (hoverAmount > 1.f ? 1.f : hoverAmount);

  sf::Color idle  = m_background_color;
  sf::Color hover = m_hover_color;

  auto lerp = [](float a, float b, float t) {
      return a + (b - a) * t;
  };

  sf::Color finalColor(
      lerp(idle.r, hover.r, hoverAmount),
      lerp(idle.g, hover.g, hoverAmount),
      lerp(idle.b, hover.b, hoverAmount),
      lerp(idle.a, hover.a, hoverAmount)
  );

  m_box.setFillColor(finalColor);
}

bool Button::mouseOnObject(const Vec2& mouse) {
    sf::Vector2f pos = getPosition();   // center
    sf::Vector2f half = m_size * 0.5f;  // half‑width, half‑height

    return (
        mouse.x >= pos.x - half.x &&
        mouse.x <= pos.x + half.x &&
        mouse.y >= pos.y - half.y &&
        mouse.y <= pos.y + half.y
    );
}

void Button::onClick( bool hovered ){
  bool pressed = sf::Mouse::isButtonPressed(sf::Mouse::Left);

  bool clicked = false;

  // detect click on release
  if ( !pressed && was_pressed_last_frame && hovered ) {
    clicked = true;
  }

  was_pressed_last_frame = pressed;

  // fire action
  if (clicked && button_action) {
      button_action();
  }
}

void Button::setButtonAction(std::function<void()> new_button_action ){
  button_action = new_button_action;
}

void Button::setOutlineColor( sf::Color new_color ){
  m_outline_color = new_color;
}
void Button::setBackgroundColor( sf::Color new_color ){
  m_background_color = new_color;
  
}
void Button::setHoverColor( sf::Color new_color ){
  m_hover_color = new_color;
}
void Button::setTextColor( sf::Color new_color ){
  m_text_color = new_color;
}