#include <UserInterface.hpp>
#include <string>
#include <iostream>

void UserInterface::InitText( const std::string& identifier, const std::string str, const uint8_t char_size, const sf::Vector2f position, const sf::Color font_color ) noexcept {
  
  std::unique_ptr<syxd::UI_Element> element = std::make_unique<syxd::Text>( m_ui_elements.size()+1,
                                                                            identifier,
                                                                            str,
                                                                            char_size,
                                                                            position,
                                                                            font_color );
  m_ui_elements.push_back( std::move(element) );

}

void UserInterface::InitInputBox( const std::string& identifier, const uint8_t char_size, const sf::Vector2f position, const sf::Color font_color  ) noexcept {
  std::unique_ptr<syxd::UI_Element> element = std::make_unique<syxd::InputBox>( m_ui_elements.size()+1, identifier, char_size, position, font_color );
  m_ui_elements.push_back( std::move(element) );

}  

void UserInterface::InitButton( const std::string identifier,
                                const sf::Vector2f position,
                                std::string text,
                                sf::Color text_color,
                                sf::Color background_color, 
                                sf::Color outline_color,
                                sf::Vector2f size,
                                sf::Vector2f padding,
                                std::function<void()> action ) noexcept {
  std::unique_ptr<syxd::UI_Element> element = std::make_unique<syxd::Button>( m_ui_elements.size()+1, 
                                                                              identifier,
                                                                              text,
                                                                              position,
                                                                              text_color,
                                                                              background_color,
                                                                              outline_color,
                                                                              size,
                                                                              padding, 
                                                                              action );
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

  if ( syxd::Text* text = dynamic_cast<syxd::Text*>( elem )  ) {
    text->setPosition( updated_position );
  }

  if ( syxd::InputBox* input_box = dynamic_cast<syxd::InputBox*>( elem )) {
    input_box->setPosition( updated_position );
  }
  
  if ( syxd::Button* button = dynamic_cast<syxd::Button*>( elem )){
    button->setPosition( updated_position );
  }

}

void UserInterface::SetElementPosition( syxd::UI_Element* elem, const std::string relative_position, const float position_offset ){
  if (WINDOW_REF == nullptr || elem == nullptr) return;

  sf::Vector2f new_position;
  sf::Vector2u window_size = WINDOW_REF->getSize();

  if ( relative_position == "top" ){
    new_position = {elem->getPosition().x, 0.0f+position_offset}; 
  }
  else if ( relative_position == "bottom" ){
    new_position = {elem->getPosition().y, window_size.y-position_offset}; 
  }

  else if ( relative_position == "center_x" ){
    
  }

  else if ( relative_position == "center_y" ){
    
  }

  else if ( relative_position == "left" ){

  }

  else if ( relative_position == "right" ){

  }

}

void UserInterface::RenderUI( const float& delta_time ){
  if ( WINDOW_REF == nullptr ) return;

  for ( const auto& elem: m_ui_elements ) { 
    if ( elem != nullptr && !elem->hidden() ) {
      if ( syxd::Text* t = dynamic_cast<syxd::Text*>( elem.get() ) ) {
        t->render( WINDOW_REF.get(), delta_time );
      }

      else if ( syxd::InputBox* i = dynamic_cast<syxd::InputBox*>( elem.get() ) ) {  
        i->render(WINDOW_REF.get(), delta_time );
      }

      else if ( syxd::Button* b = dynamic_cast<syxd::Button*>( elem.get() ) ) {  
        b->render(WINDOW_REF.get(), delta_time );
      }

    }
  }
}

bool UserInterface::SetFont( const sf::Font& font ) noexcept {
  this->font = font;
  return true;
}

sf::Font UserInterface::GetFont(){
  return font;
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

