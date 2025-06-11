#include <UserInterface.hpp> 
#include <string>
#include <iostream>

void UserInterface::InitText( const std::string& identifier, const std::string str, const uint8_t char_size, const sf::Vector2f position, const sf::Color font_color ) noexcept {
  std::unique_ptr<syxd::UI_Element> element = std::make_unique<syxd::Text>( m_ui_elements.size()+1, identifier, str, char_size, position, font_color );
  m_ui_elements.push_back( std::move(element) );

}

void UserInterface::InitInputBox( const std::string& identifier, const uint8_t char_size, const sf::Vector2f position, const sf::Color font_color  ) noexcept {
  std::unique_ptr<syxd::UI_Element> element = std::make_unique<syxd::InputBox>( m_ui_elements.size()+1, identifier, char_size, position, font_color );
  m_ui_elements.push_back( std::move(element) );

}  


bool UserInterface::AddElement( std::unique_ptr<syxd::UI_Element> elem ) {
  m_ui_elements.push_back( std::move(elem) );
  return true;
}

std::unique_ptr<syxd::UI_Element>::pointer UserInterface::FindElement( const std::string& element_identifier ) const noexcept {
  for ( auto& elem : m_ui_elements ) {
    if (elem == nullptr) continue;

    if ( elem->getIdentifier() == element_identifier ){
      return elem.get();
    }
  }
  return nullptr;
}

std::unique_ptr<syxd::UI_Element>::pointer UserInterface::FindElement( const uint32_t element_id ) const noexcept {
  for ( auto& elem : m_ui_elements ) {
    if (elem == nullptr) continue;

    if ( elem->getID() == element_id ){
      return elem.get();
    }
  }
  return nullptr;
}


void UserInterface::UpdateElementText( syxd::UI_Element* elem, const std::string& updated_string ) {
  if ( elem == nullptr || !elem ) return;

  if ( syxd::Text* t = dynamic_cast<syxd::Text*>( elem )  ) {
    t->setString( updated_string );
  }
}

void UserInterface::UpdateElementPosition( syxd::UI_Element* elem, const sf::Vector2f& updated_position ) {
  if ( elem == nullptr || !elem ) return;

  if ( syxd::Text* t = dynamic_cast<syxd::Text*>( elem )  ) {
    t->setPosition( updated_position );
  }
}

void UserInterface::SetElementPosition( UI_Element_Legacy* elem, const std::string relative_position, const float position_offset ){
  if (WINDOW_REF == nullptr || elem == nullptr) return;

  sf::Vector2f new_position;
  sf::Vector2u window_size = WINDOW_REF->getSize();

  if ( relative_position == "top" ){
    new_position = {elem->position.x, 0.0f+position_offset}; 
  }
  else if ( relative_position == "bottom" ){
    new_position = {elem->position.x, window_size.y-position_offset}; 
  }

  else if ( relative_position == "center_x" ){
    
  }

  else if ( relative_position == "center_y" ){
    
  }

  else if ( relative_position == "left" ){

  }

  else if ( relative_position == "right" ){

  }

  elem->position = new_position;
  elem->text.setPosition(elem->position);

}

void UserInterface::RenderUI( const float& delta_time ){
  if ( WINDOW_REF == nullptr ) return;

  for ( const auto& elem: m_ui_elements ) { 
    if ( elem != nullptr && !elem->hidden() ) {
      if ( syxd::Text* t = dynamic_cast<syxd::Text*>( elem.get() ) ) {
        WINDOW_REF->draw( t->getTextElement() );
      }

      else if ( syxd::InputBox* t = dynamic_cast<syxd::InputBox*>( elem.get() ) ) {  
        t->render(WINDOW_REF, delta_time );
      }

    }
  }
}

bool UserInterface::SetFont( const sf::Font& font ) noexcept {
  this->font = font;
  return true;
}

void UserInterface::ShowElement( syxd::UI_Element* target_element ) {
  if (target_element != nullptr) target_element->hide(false);
}

void UserInterface::HideElement( syxd::UI_Element* target_element ) {
  if (target_element != nullptr) target_element->hide(true);
}

void UserInterface::HideAllElements( ){
  for ( auto& elem : m_ui_elements ){
    HideElement(elem.get());
  }
}

void UserInterface::ShowAllElements( ){
  for ( auto& elem : m_ui_elements ){
    ShowElement(elem.get());
  }
}

std::shared_ptr<sf::RenderWindow> UserInterface::GetWindow( ){
  return WINDOW_REF;
}
void UserInterface::SetWindow( std::shared_ptr<sf::RenderWindow> window ){
  WINDOW_REF = window;
}

