#include <memory> 
#include <SFML/Graphics.hpp>
#include <array>
#include <string>
#include <cassert>
#include <UIElements.hpp>

class UserInterface {
private:
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
 
  enum font_size {
    H1,
    H2,
    H3,
    H4,
    P
  };
  
  void InitText( const std::string& identifier, const std::string str, const uint8_t char_size, const sf::Vector2f position, const sf::Color font_color  ) noexcept;  
  void InitInputBox( const std::string& identifier, const uint8_t char_size, const sf::Vector2f position, const sf::Color font_color  ) noexcept;  
  
  bool AddElement( std::unique_ptr<syxd::UI_Element> elem );
  void RenderUI( const float& delta_time );
  void UpdateElementPosition(  syxd::UI_Element* elem, const sf::Vector2f& updated_position );
  void UpdateElementText( syxd::UI_Element* elem, const std::string& updated_string );
  void SetElementPosition( syxd::UI_Element* elem, const std::string relative_position, const float position_offset );
  bool SetFont( const sf::Font& font ) noexcept;
  void HideElement( syxd::UI_Element* target_element );
  void ShowElement( syxd::UI_Element* target_element );
  void HideAllElements();
  void ShowAllElements();
  std::shared_ptr<sf::RenderWindow> GetWindow( );
  void SetWindow( std::shared_ptr<sf::RenderWindow> window );
  std::unique_ptr<syxd::UI_Element>::pointer FindElement( const std::string& element_identifier ) const noexcept;
  std::unique_ptr<syxd::UI_Element>::pointer FindElement( const uint32_t element_id ) const noexcept;

};