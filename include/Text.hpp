#pragma once
#include "UIElements.hpp"

namespace syxd {
  using namespace syxd;
  
  class Text : public UI_Element {
    private:
    sf::Text m_text;
    sf::Color m_text_color = sf::Color::Red;
    sf::Font m_font;
    uint8_t m_char_size = 20;

    public:
    Text () { };
    Text ( const uint32_t id, 
          const std::string identifier,
          const std::string text,
          uint8_t char_size,
          const sf::Vector2f position,
          sf::Color text_color ) 
    : UI_Element( id, identifier, position ) 
    {
      m_char_size = char_size;
      m_text_color = text_color;
      setFontDefault();
      
      m_text.setString( text );
      m_text.setFont( m_font );
      m_text.setCharacterSize( m_char_size );
      m_text.setColor( m_text_color );
      m_text.setFillColor( m_text_color );
      m_text.setPosition( position );
      setSize( m_text.getLocalBounds().width, m_text.getLocalBounds().height );

    };
    ~Text(){};

    void setString( const std::string& updated_str ){
      m_text.setString( updated_str );
    }

    void setPosition( const sf::Vector2f& position ){
      UI_Element::setPosition(position);
      m_text.setPosition( position );
    }

    void setPosition( const float& pos_x, const float& pos_y ){
      UI_Element::setPosition(pos_x, pos_y);
      m_text.setPosition( pos_x, pos_y );
    }

    void setPosition( const Vector2<float>& position ){
      UI_Element::setPosition(position);
      m_text.setPosition( position.x, position.y );
    }

    void setCharacterSize( const float& char_size ){
      m_char_size = char_size;
    }

    void setTextColor( const sf::Color& text_color ){
      m_text_color = text_color;
    }

    sf::Text getTextElement(){
      return m_text;
    };

    void setFont( const std::string path ) {
      if ( !m_font.loadFromFile( path ) ){
        std::cout << "Font not found!\n";
      }
    }

    void setFontDefault( ) {
      if ( !m_font.loadFromFile( "static/fonts/cairo.ttf" ) ){
        std::cout << "Font not found!\n";
      }
    }

    void render( sf::RenderWindow* WINDOW_REF, const float& delta_time ) override {
      WINDOW_REF->draw( getTextElement() );
    }
   
  };

}
