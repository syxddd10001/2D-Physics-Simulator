#include <memory> 
#include <SFML/Graphics.hpp>
#include <array>
#include <string>
#include <cassert>


struct UI_Element {
  std::string identifier;
  sf::Text text;
  uint8_t char_size;
  sf::Vector2f position;
  
  UI_Element( const std::string& identifier, const sf::Text& text, uint8_t char_size, sf::Vector2f position)
        :identifier(identifier) , text(text), char_size(char_size), position(position) {}
};

class UserInterface {
private:
  std::vector<std::unique_ptr<UI_Element>> m_ui_elements;
  sf::Font font;
  const uint8_t h1_size = 50; // 50
  const uint8_t h2_size = 35; // 35
  const uint8_t h3_size = 25; // 25
  const uint8_t p1_size = 20; // 20

public:
  UserInterface(){};
  ~UserInterface(){};
  void InitElement( const std::string identifier, const std::string str, const uint8_t char_size, const sf::Vector2f position ) noexcept;
  bool AddElement( std::unique_ptr<UI_Element> elem );
  void RenderUI( std::shared_ptr<sf::RenderWindow> WINDOW );
  void UpdateElementPosition(  UI_Element* elem, const sf::Vector2f& updated_position );
  void UpdateElementText( UI_Element* elem, const std::string& updated_string );
  bool SetFont( const sf::Font& font ) noexcept;
  std::unique_ptr<UI_Element>::pointer FindElement( const std::string& element_identifier ) const noexcept;
  
};