#pragma once
#include <UIElements.hpp>
bool action = false;
syxd::UI_Element::UI_Element ( const uint32_t id, const std::string identifier, const sf::Vector2f position ) noexcept
  : m_id(id), m_identifier(identifier), m_position(position)
{
  std::cout << identifier << " created in UI\n";

}

void syxd::InputBox::checkInput( sf::Event e_event, std::shared_ptr<sf::RenderWindow> WINDOW, const float& delta_time ) {
  if ( m_focused ) {
    if ( e_event.type == sf::Event::TextEntered ) {
      if ( std::isprint(e_event.text.unicode) 
          && (m_input_text.findCharacterPos(m_input_string.size()).x) < m_text_box.getSize().x) {
        m_elapsed_time_input = 0.0f;
        char ch = static_cast<char>( e_event.text.unicode );
        m_input_string.insert(m_cursor_position, 1, ch);
        m_cursor_position_history.push(m_cursor_position);
        m_cursor_position++;
      }
    }
    

    if (e_event.type == sf::Event::KeyPressed) {
      if ( e_event.key.control &&
        e_event.key.code == sf::Keyboard::V
      ) {
        std::string clipboard = sf::Clipboard::getString();
        clipboard.erase(std::remove(clipboard.begin(), clipboard.end(), '\n'), clipboard.end());
        m_input_string.insert(m_cursor_position, clipboard);
        m_cursor_position += clipboard.length();
      }

      if ( e_event.key.control &&
        e_event.key.code == sf::Keyboard::Z && 
        !m_cursor_position_history.empty() ) {
          uint8_t top = m_cursor_position_history.top();
          m_input_string.erase(top,1);
          m_cursor_position_history.pop();
          m_cursor_position_history.empty() ? m_cursor_position = 0 : m_cursor_position = m_cursor_position_history.top()+1;
      }

      if ( e_event.key.code == sf::Keyboard::Enter ) { 
        if ( m_clear_on_enter ) {

          clearInput();
        }
      }

      if ( e_event.key.control && e_event.key.code == sf::Keyboard::P ) {
        m_input_string = m_input_previous;
      }

      if ( e_event.key.code == sf::Keyboard::Left && 
        m_cursor_position > 0 &&
        m_elapsed_time_input >= _INPUT_INTERVAL ) {
          m_cursor_position --;
      }

      if ( e_event.key.code == sf::Keyboard::Right &&
      m_cursor_position < m_input_string.size() &&
      m_elapsed_time_input >= _INPUT_INTERVAL ) {
        m_cursor_position ++;  
      }

      if ( e_event.key.code == sf::Keyboard::BackSpace ) {
        if ( !m_input_string.empty() && m_cursor_position > 0 ) {    
          m_input_string.erase(m_cursor_position-1,1);
          m_cursor_position--;
        }
      }

      if ( e_event.key.code == sf::Keyboard::Delete ) {
        if ( !m_input_string.empty() && m_cursor_position != m_input_string.length() ) {    
          m_input_string.erase(m_input_string.begin()+m_cursor_position,m_input_string.begin()+m_cursor_position+1);
        }
      }

      if ( e_event.key.code == sf::Keyboard::End ) {
        m_cursor_position = m_input_string.size();
      }
      if ( e_event.key.code == sf::Keyboard::Home ) {
        m_cursor_position = 0;
      }


      if ( e_event.key.control && e_event.key.code == sf::Keyboard::BackSpace ) {
        clearInput();
      }

    }

  }

  if (e_event.type == sf::Event::KeyReleased){
    if ( e_event.key.code == sf::Keyboard::Escape ) {
      if ( m_focused ) {
        if ( !m_input_string.empty() && m_cursor_position > 0) {    
          m_input_string.clear();
          m_cursor_position = 0;
        }
        
        if ( m_input_string.empty() ){
          m_focused = false;
          m_cursor.setFillColor( sf::Color::Transparent );
        }
      }  
    }
  }

  
}

void syxd::InputBox::render( std::shared_ptr<sf::RenderWindow> WINDOW, const float& delta_time ) {
  ( m_elapsed_time_cursor_blink < _CURSOR_BLINK_INTERVAL ) ? m_elapsed_time_cursor_blink += delta_time : m_elapsed_time_cursor_blink = _CURSOR_BLINK_INTERVAL;
  ( m_elapsed_time_input < _INPUT_INTERVAL ) ? m_elapsed_time_input += delta_time : m_elapsed_time_input = _INPUT_INTERVAL;

  if ( m_focused ){
    if ( m_show_cursor && m_elapsed_time_cursor_blink >= _CURSOR_BLINK_INTERVAL ) {
      m_show_cursor = false;
      m_elapsed_time_cursor_blink = 0.0f;
    }
  
    if ( !m_show_cursor && m_elapsed_time_cursor_blink >= _CURSOR_BLINK_INTERVAL ) {
      m_show_cursor = true;
      m_elapsed_time_cursor_blink = 0.0f;
    }


  }
  
  if ( m_show_cursor && m_focused ) {
    m_cursor.setFillColor( sf::Color::White );
    m_text_color = sf::Color::White;
  }
  else {
    m_cursor.setFillColor( sf::Color::Transparent );
    m_text_color = sf::Color::Transparent;

  }

  std::string visibleText = m_input_string;
  
  m_input_text.setString(visibleText);
  m_cursor.setPosition(sf::Vector2f { std::min((m_input_text.findCharacterPos(m_cursor_position).x),(m_input_text.findCharacterPos(m_input_string.size()).x)), 
                                    m_input_text.findCharacterPos(m_cursor_position).y + 4.f });
                                 

  WINDOW->draw( m_text_box );
  WINDOW->draw( getTextElement() );
  WINDOW->draw( m_cursor );
}

void syxd::InputBox::setInputBoxSize( const sf::Vector2f size ){
    m_text_box.setSize( size ); 
    max_visible_chars = std::max( 1, static_cast<int>( m_text_box.getSize().x / average_char_width ) );
    std::cout << average_char_width << "\n";
    std::cout << max_visible_chars << "\n";

}