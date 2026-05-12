#pragma once
#include <memory> 
#include <SFML/Graphics.hpp>
#include <array>
#include <string>
#include <cassert>
#include <UIElements.hpp>
#include <Text.hpp>
#include <InputBox.hpp>
#include <Button.hpp>

class UserInterface {
private:
  std::vector<std::unique_ptr<syxd::UI_Element>> m_ui_elements;

  sf::Font font;
  static constexpr  uint8_t h1_size = 50; // 50
  static constexpr  uint8_t h2_size = 35; // 35
  static constexpr  uint8_t h3_size = 25; // 25
  static constexpr  uint8_t p1_size = 20; // 20

  std::shared_ptr<sf::RenderWindow> WINDOW_REF; 

public:
  UserInterface(){};
  ~UserInterface(){};
  UserInterface(const UserInterface&) = delete;
  UserInterface& operator=(const UserInterface&) = delete;
  UserInterface(UserInterface&&) = default;
  UserInterface& operator=(UserInterface&&) = default;

  bool operator!() const noexcept {
    bool no_elements = m_ui_elements.empty();
    bool no_window   = !WINDOW_REF;
    bool no_font     = font.getInfo().family.empty();

    return no_elements && no_window && no_font;
  }

  explicit operator bool() const noexcept {
    return !(*this);   // UI is valid when NOT empty
  }


  enum font_size {
    H1,
    H2,
    H3,
    H4,
    P
  };
  
  void InitText( const std::string& identifier, 
                  const std::string str, 
                  const uint8_t char_size, 
                  const sf::Vector2f position, 
                  const sf::Color font_color  ) noexcept;

  void InitInputBox( const std::string& identifier, 
                      const uint8_t char_size, 
                      const sf::Vector2f position, 
                      const sf::Color font_color  ) noexcept;  

  void InitButton( const std::string identifier,
                    const sf::Vector2f position,
                    std::string text,
                    sf::Color text_color,
                    sf::Color background_color,
                    sf::Color hover_color,
                    sf::Color outline_color,
                    sf::Vector2f size,
                    sf::Vector2f padding,
                    bool hover,
                    std::function<void()> action ) noexcept; 

  bool AddElement( std::unique_ptr<syxd::UI_Element> elem );
  void RenderUI( const float& delta_time );
  void UpdateElementPosition(  syxd::UI_Element* elem, const sf::Vector2f& updated_position );
  void UpdateElementText( syxd::UI_Element* elem, const std::string& updated_string );
  void SetElementPosition( syxd::UI_Element* elem, const std::string relative_position, const float position_offset );
  bool SetFont( const sf::Font& font ) noexcept;
  sf::Font GetFont ();
  void HideElement( syxd::UI_Element* target_element );
  void ShowElement( syxd::UI_Element* target_element );
  void HideAllElements();
  void ShowAllElements();
  std::shared_ptr<sf::RenderWindow> GetWindow( );
  void SetWindow( std::shared_ptr<sf::RenderWindow> window );
  std::unique_ptr<syxd::UI_Element>::pointer FindElement( const std::string& element_identifier ) const noexcept;
  std::unique_ptr<syxd::UI_Element>::pointer FindElement( const uint32_t element_id ) const noexcept;
  void RemoveAllElements();
  void resizeUI(const sf::Vector2f& prev_size, const sf::Vector2u& new_size);
    
};