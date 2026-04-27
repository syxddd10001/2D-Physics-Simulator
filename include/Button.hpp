#pragma once
#include <UIElements.hpp>

namespace syxd{
  class Button : public UI_Element {
    private:
    sf::Text m_text;
    sf::Color m_text_color = sf::Color::Red;
    sf::Font m_font;
    sf::RectangleShape m_box;
    sf::Color m_background_color;
    sf::Color m_outline_color = sf::Color::White;
    float pad_x;
    float pad_y;
    bool m_hover;
    sf::Vector2f m_size;
    std::function<void()> button_action;

    public:
    Button ( const uint32_t id, 
          const std::string identifier,
          const std::string text,
          const sf::Vector2f position,
          sf::Color text_color,
          sf::Color background_color, 
          sf::Color outline_color,
          sf::Vector2f size,
          sf::Vector2f padding,
          std::function<void()> action
        ) 
    : UI_Element( id, identifier, position ) {
      setFontDefault();
      m_text.setFont(m_font);
      m_text_color = text_color;
      m_background_color = background_color;
      m_outline_color = outline_color;
      m_size = size;
      pad_x = padding.x;
      pad_y = padding.y;
      m_box = sf::RectangleShape(size);
      m_box.setPosition(position);
      sf::FloatRect bounds = m_text.getLocalBounds();
      m_text.setOrigin(bounds.left + bounds.width / 2.f, bounds.top  + bounds.height / 2.f);
      m_text.setPosition(position.x + m_size.x / 2.f,
                        position.y + m_size.y / 2.f);

      m_text.setFillColor(text_color);
      std::cout << m_text.getColor().toInteger() << std::endl;
      std::cout << m_text.getPosition().x << "," << m_text.getPosition().y << std::endl;



      m_box.setSize(size);
      m_box.setFillColor(background_color);
      m_box.setOutlineColor(outline_color);
      m_box.setOutlineThickness(1);
      
      m_text.setString(text);
      m_text.setCharacterSize(size.y*0.35);


    };
    ~Button(){};

    void setFont( const std::string path ) {
      if ( !m_font.loadFromFile( path ) ){
        std::cout << "Font not found!\n";
        return;
      }
      
      m_text.setFont(m_font);
      
    }

    void setFontDefault( ) { // THIS IS BAD FIX THIS
      if ( !m_font.loadFromFile( "static/fonts/cairo.ttf" ) ){
        std::cout << "Font not found!\n";
        return;
      }
      std::cout << "font set successfully\n";
    }

    void setPosition( const sf::Vector2f& position ){
      UI_Element::setPosition(position);
      m_box.setPosition( position );
      m_text.setPosition( (position.x+m_size.x)/2, (position.y+m_size.y)/2);
    }

    void render(sf::RenderWindow* WINDOW_REF, const float& delta_time) override {
      WINDOW_REF->draw(m_box);
      WINDOW_REF->draw(m_text);
    }
    
  };

}