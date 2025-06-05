#include <SFML/Graphics.hpp>
#include <Vector2.hpp>
#include <memory> 
#include <array>
#include <string>
#include <iostream>

namespace syxd{
  using namespace syxd;
  
  class UI_Element {
  private:
    uint32_t m_id;
    std::string m_identifier;
    sf::Vector2f m_position;
    sf::Vector2f m_size;
    bool m_hidden = false;
    std::string m_type;

  public:
    UI_Element ( const uint32_t id, const std::string identifier, const sf::Vector2f position ) noexcept;
    UI_Element ( ) { };
    virtual ~UI_Element ( ) = default;
    
    const std::string getIdentifier(){
      return m_identifier;
    } 

    void setSize( const sf::Vector2f& size ){
      m_size = size;
    }
    void setSize( const float& size_x, const float& size_y ) {
      m_size = { size_x, size_y };
    }
    void setSize( const Vector2<float>& size ) {
      m_size = size;
    }
  
    void setPosition( const sf::Vector2f& position ) {
      m_position = position;
    }
    void setPosition( const float& pos_x, const float& pos_y ) {
      m_position = { pos_x, pos_y };
    }
    void setPosition( const Vector2<float>& position ) {
      m_size = position;
    }

    void hide( const bool& b ){
      m_hidden = b;
    }

    bool hidden( ){
      return m_hidden;
    }

    std::string what_type( ){
      return m_type;
    };
  };

  class Text : public UI_Element {
    private:    
    sf::Text m_text;
    sf::Color m_text_color = sf::Color::White;
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
      setFontDefault();
      m_text.setString( text );
      m_text.setFont( m_font );
      m_text.setCharacterSize( m_char_size );
      m_text.setColor( m_text_color );
      m_text.setPosition( position );
      setSize( m_text.getLocalBounds().width, m_text.getLocalBounds().height );

    };

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

    sf::Text text_element(){
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
  
    
  };

  class InputBox : public UI_Element {
    
  };

  class Button : public UI_Element {

  };

}