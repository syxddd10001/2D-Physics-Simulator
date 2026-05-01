#pragma once
#include <algorithm>

#include <UIElements.hpp>
#include <PhysicsMath.hpp>

namespace syxd{
  class Button : public UI_Element {
    private:
    sf::Text m_text;
    sf::Color m_text_color = sf::Color::Red; // default text color
    sf::Font m_font;
    sf::RectangleShape m_box;
    sf::Color m_background_color = sf::Color::Blue; // default background color
    sf::Color m_hover_color =  sf::Color::Green; // default hover color

    sf::Color m_outline_color = sf::Color::White; // default outline color
    float pad_x;
    float pad_y;
    bool m_hover;
    sf::Vector2f m_size;
    std::function<void()> button_action;
    float hoverAmount = 0.f;
    bool was_pressed_last_frame = false;

    public:
    Button ( const uint32_t id, 
          const std::string identifier,
          const std::string text,
          const sf::Vector2f position,
          sf::Color text_color,
          sf::Color background_color,
          sf::Color hover_color,
          sf::Color outline_color,
          sf::Vector2f size,
          sf::Vector2f padding,
          bool hover,
          std::function<void()> action
        ) 
    : UI_Element( id, identifier, position ) {
      setFontDefault();
      m_text.setFont(m_font);
      m_text.setString(text);
      m_text.setCharacterSize(size.y*0.35);
      m_text.setStyle(sf::Text::Bold);
      m_text_color = text_color;
      m_text.setFillColor(text_color);

      sf::FloatRect bounds = m_text.getLocalBounds();
      m_text.setOrigin(
          bounds.left + bounds.width  / 2.f,
          bounds.top  + bounds.height / 2.f
      );
      m_text.setPosition(position);

      m_background_color = background_color;
      m_outline_color = outline_color;
      m_hover_color = hover_color;
      m_hover = hover;
      m_size = size;
      pad_x = padding.x;
      pad_y = padding.y;
      m_box = sf::RectangleShape(size);
      m_box.setSize(size);
            
      m_box.setOrigin(size.x / 2.f, size.y / 2.f);
      m_box.setPosition(position);   

      std::cout << m_text.getColor().toInteger() << std::endl;
      std::cout << m_text.getPosition().x << "," << m_text.getPosition().y << std::endl;

      m_box.setFillColor(background_color);
      m_box.setOutlineColor(outline_color);
      m_box.setOutlineThickness(1);

      button_action = action;

    };
    ~Button(){};

    void setFont( const std::string path );
    void setFontDefault( );
    void setPosition( const sf::Vector2f& position );
    void setButtonAction( std::function<void()> new_button_action );
    void setOutlineColor( sf::Color new_color );
    void setBackgroundColor( sf::Color new_color );
    void setHoverColor( sf::Color new_color );
    void setTextColor( sf::Color new_color );
    void onHover( sf::Vector2f mouse_pos, bool hovered, const float& delta_time );
    bool mouseOnObject(const Vec2& mouse);
    void onClick( bool hovered );
    void render(sf::RenderWindow* WINDOW_REF, const float& delta_time ) override;


    
  };

}