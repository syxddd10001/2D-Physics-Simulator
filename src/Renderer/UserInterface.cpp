#include <UserInterface.hpp> 

void UserInterface::InitElement( const std::string identifier, const std::string str, const uint8_t char_size, const sf::Vector2f position ) noexcept { 
  sf::Text text;
  text.setFont( font );
  text.setString( str );
  text.setPosition( position );
  text.setCharacterSize(char_size);
  
  std::unique_ptr<UI_Element> elem = std::make_unique<UI_Element>( identifier, text, char_size, position ); 
  
  AddElement( std::move(elem) );
  
}

bool UserInterface::AddElement( std::unique_ptr<UI_Element> elem ){
  m_ui_elements.push_back( std::move(elem) );
  return true;
}

std::unique_ptr<UI_Element>::pointer UserInterface::FindElement( const std::string& element_identifier ) const noexcept {
  for ( auto& elem : m_ui_elements ){
    if ( elem->identifier == element_identifier ){
      return elem.get();
    }
  }
  return nullptr;
} 

void UserInterface::UpdateElementText( UI_Element* elem, const std::string& updated_string ){
  if (elem) elem->text.setString(updated_string);
}

void UserInterface::UpdateElementPosition( UI_Element* elem, const sf::Vector2f& updated_position ){
  if (elem) {
    elem->position = updated_position;
    elem->text.setPosition( elem->position );
  }
}

void UserInterface::RenderUI( std::shared_ptr<sf::RenderWindow> WINDOW ){
  for (const auto& elem: m_ui_elements){
    WINDOW->draw( elem->text );
  }
}

bool UserInterface::SetFont( const sf::Font& font ) noexcept {
  this->font = font;
  return true;
}