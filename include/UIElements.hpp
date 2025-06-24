#include <SFML/Graphics.hpp>
#include <Vector2.hpp>
#include <memory> 
#include <array>
#include <string>
#include <iostream>
#include <stack>
#include <algorithm>

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
    
    const uint32_t getID(){
      return m_id;
    } 

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

    sf::Vector2f getPosition( ) {
      return m_position;
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
  
    
  };

  class InputBox : public UI_Element {
    int8_t m_cursor_position = 0;
    std::string m_input_string = "";
    std::stack<int8_t> m_cursor_position_history;

    
    sf::Text m_input_text;
    sf::RectangleShape m_cursor;
    std::string m_input_previous = "";
    sf::Font m_font;
    uint8_t m_char_size = 20;
    sf::Color m_text_color = sf::Color::White;
    sf::Color m_background_color;
    sf::Color m_outline_color = sf::Color::White;

    int max_visible_chars;
    float average_char_width;

    bool m_clear_on_enter = false;
    bool m_focused = false;
    float _CURSOR_BLINK_INTERVAL = 0.50f;
    float _INPUT_INTERVAL = 0.0001f;

    float pad_x;
    float pad_y;

    float m_elapsed_time_cursor_blink = _CURSOR_BLINK_INTERVAL;
    float m_elapsed_time_input = _INPUT_INTERVAL;
    bool m_show_cursor = false;

    sf::RectangleShape m_text_box;
    

    public:
    InputBox () { };
    InputBox ( const uint32_t id, 
          const std::string identifier,
          uint8_t char_size,
          const sf::Vector2f position,
          sf::Color text_color ) 
    : UI_Element( id, identifier, position ) 
    {
      m_char_size = char_size;
      m_text_color = text_color;
      UI_Element::setPosition(position);
      setFontDefault();
    
      m_input_text = sf::Text( "X ", m_font ); // the input box
      m_input_text.setCharacterSize( m_char_size );
      m_input_text.setColor( m_text_color );
      m_input_text.setPosition( position );   

      setSize( m_input_text.getLocalBounds().width, m_input_text.getLocalBounds().height );

      m_cursor.setSize( sf::Vector2f { 5.0f, char_size } ); // cursor for text input
      m_cursor.setFillColor( sf::Color::White );
      m_cursor.setPosition( getPosition() );

      m_text_box.setSize( sf::Vector2f { 5.0f, char_size } );
      m_text_box.setPosition( getPosition() );
      m_text_box.setOutlineThickness( 1.0f );

      average_char_width = getAverageCharWidth();
      max_visible_chars = std::max(1, static_cast<int>(m_text_box.getSize().x / average_char_width));

      std::cout << m_input_text.findCharacterPos(m_cursor_position).x << ", " << m_input_text.findCharacterPos(m_cursor_position).x << "\n";

    };

    sf::Vector2f getTextBoxSize(){
      return m_text_box.getSize();
    }

    sf::RectangleShape getTextBox(){
      return m_text_box;
    }

    void setCursorSize( const sf::Vector2f& new_size ){
      m_cursor.setSize(new_size);

    }

    void setPadding( const sf::Vector2f& new_pad ){
      pad_x = new_pad.x;
      pad_y = new_pad.y;
    }

    void setPaddingX( const float& X ){
      pad_x = X;
    }

    void setPaddingY( const float& Y ){
      pad_y = Y;
    }

    sf::Color getBackgroundColor() const{
      return m_background_color;
    } 

    void setBackgroundColor( const sf::Color& new_color ){
      m_background_color = new_color;
      m_text_box.setFillColor( new_color );
    }

    sf::Color getOutlineColor() const {
      return m_outline_color;
    } 

    void setOutlineColor( const sf::Color& new_color ){
      m_outline_color = new_color;
      m_text_box.setOutlineColor( new_color );
    }

    bool isFocused() const {
      return m_focused;
    }

    void setFocused( const bool& b ){
      m_focused = b;
    }

    std::string getInputText(){
      return m_input_string;
    }

    void clearTextOnEnter( const bool& b ){
      m_clear_on_enter = b;
    }

    void clearInput(){
      if ( !m_input_string.empty() && m_cursor_position > 0) {
        m_input_previous = m_input_string;
        m_input_string.clear();
        m_cursor_position = 0;
      }
    }
    
    void setFont( const std::string path ) {
      if ( !m_font.loadFromFile( path ) ){
        std::cout << "Font not found!\n";
        return;
      }
      
      m_input_text.setFont(m_font);
      
    }

    void setFontDefault( ) {
      if ( !m_font.loadFromFile( "static/fonts/cairo.ttf" ) ){
        std::cout << "Font not found!\n";
        return;
      }
      std::cout << "font set successfully\n";
    }

    sf::Text getTextElement(){
      return m_input_text;
    }

    sf::RectangleShape getCursor(){
      return m_cursor;
    }

    float getAverageCharWidth(std::string chars="ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"){
      float total_size = 0;
      for ( char c : chars ){
        total_size += m_input_text.getFont()->getGlyph(c, m_input_text.getCharacterSize(), false).advance;
      }

      return (total_size / (26*2));
    }

    void setTextString(std::string target){
      m_input_string = target;
      m_input_text.setString(target);
    }

    void render( std::shared_ptr<sf::RenderWindow> WINDOW, const float& delta_time );
    
    void checkInput( sf::Event e_event, std::shared_ptr<sf::RenderWindow> WINDOW, const float& delta_time );

    void setInputBoxSize( const sf::Vector2f size );
  };

  class Button : public UI_Element {

  };

}