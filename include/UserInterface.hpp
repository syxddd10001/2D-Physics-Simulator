#include <memory> 
#include <SFML/Graphics.hpp>
#include <array>
#include <string>
#include <cassert>
#include <UIElements.hpp>

struct UI_Element_Legacy {
  std::string identifier;
  sf::Text text;
  uint8_t char_size;
  sf::Vector2f position;
  bool show_element = true;
  
  UI_Element_Legacy( const std::string& identifier, const sf::Text& text, uint8_t char_size, sf::Vector2f position)
        :identifier(identifier) , text(text), char_size(char_size), position(position) {}

};

class UserInterface {
private:
  std::vector<std::unique_ptr<UI_Element_Legacy>> m_ui_elements_legacy;
  std::vector<std::unique_ptr<syxd::UI_Element>> m_ui_elements;

  sf::Font font;
  const uint8_t h1_size = 50; // 50
  const uint8_t h2_size = 35; // 35
  const uint8_t h3_size = 25; // 25
  const uint8_t p1_size = 20; // 20

  std::shared_ptr<sf::RenderWindow> WINDOW_REF; 

public:
  UserInterface(){};
  ~UserInterface(){};
 
  
  void InitElement( const std::string& identifier, const std::string str, const uint8_t char_size, const sf::Vector2f position, const sf::Color font_color  ) noexcept;  
  bool AddElement( std::unique_ptr<syxd::UI_Element> elem );
  void RenderUI( );
  void UpdateElementPosition(  syxd::UI_Element* elem, const sf::Vector2f& updated_position );
  void UpdateElementText( syxd::UI_Element* elem, const std::string& updated_string );
  void SetElementPosition( UI_Element_Legacy* elem, const std::string relative_position, const float position_offset );
  bool SetFont( const sf::Font& font ) noexcept;
  void HideElement( syxd::UI_Element* target_element );
  void ShowElement( syxd::UI_Element* target_element );
  void HideAllElements();
  void ShowAllElements();
  std::shared_ptr<sf::RenderWindow> GetWindow( );
  void SetWindow( std::shared_ptr<sf::RenderWindow> window );
  std::unique_ptr<syxd::UI_Element>::pointer FindElement( const std::string& element_identifier ) const noexcept;
  

  void InitElement_Legacy( const std::string identifier, const std::string str, const uint8_t char_size, const sf::Vector2f position, const sf::Color& text_color ) noexcept;  
  void RenderUI_Legacy( );
  bool AddElement_Legacy( std::unique_ptr<UI_Element_Legacy> elem );
  void UpdateElementPosition_Legacy(  UI_Element_Legacy* elem, const sf::Vector2f& updated_position );
  void UpdateElementText_Legacy( UI_Element_Legacy* elem, const std::string& updated_string );

  void HideElement_Legacy( UI_Element_Legacy* target_element );
  void ShowElement_Legacy( UI_Element_Legacy* target_element );
  std::unique_ptr<UI_Element_Legacy>::pointer FindElement_Legacy( const std::string& element_identifier ) const noexcept;


};