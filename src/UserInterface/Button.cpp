#include <Button.hpp>

void Button::setFont( const std::string path ) {
  if ( !m_font.loadFromFile( path ) ){
    DEBUG_PRINT("Font not found!\n");
    return;
  }
  
  m_text.setFont(m_font);
  
}

void Button::setFontDefault( ) { // THIS IS BAD FIX THIS
  if ( !m_font.loadFromFile( "static/fonts/cairo.ttf" ) ){
    DEBUG_PRINT("Font not found!\n");
    return;
  }
  DEBUG_PRINT("font set successfully\n");
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

void Button::setCharacterSize( const float size ){
  m_text.setCharacterSize(size);
  sf::FloatRect bounds = m_text.getLocalBounds();
  m_text.setOrigin(
      bounds.left + bounds.width  / 2.f,
      bounds.top  + bounds.height / 2.f
  );
  m_text.setPosition(getPosition());
}

sf::RectangleShape Button::getBox( ) const { 
  return m_box; 
}


std::string mediaQuery( const sf::Vector2u& view_size ) {
  if ( view_size.x <= 480 ) return "small";
  if ( view_size.x > 480 && view_size.x <= 768 ) return "medium";
  if ( view_size.x > 768 && view_size.x <= 1600 ) return "large";
  if ( view_size.x > 1600 ) return "extra-large";
  else return "unknown";
}

void Button::resizeUI(const sf::Vector2f& prev_view_size,
                      const sf::Vector2u& new_view_size) {
  // Compute relative position
  float relX = m_box.getPosition().x / prev_view_size.x;
  float relY = m_box.getPosition().y / prev_view_size.y;

  // Apply to new view
  float newX = relX * new_view_size.x;
  float newY = relY * new_view_size.y;

  // Update
  m_box.setPosition(newX, newY);
  m_text.setPosition(newX, newY);
  setPosition(Vec2(newX, newY));

  // Typography scaling
  std::string mq = mediaQuery(new_view_size);

  if (mq == "small") {
      setCharacterSize(12);
  } else if (mq == "medium" || mq == "large") {
      setCharacterSize(m_box.getSize().y * 0.35f);
  } else {
      setCharacterSize(m_box.getSize().y * 0.40f);
  }
}