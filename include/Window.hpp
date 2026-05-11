#pragma once
#include <memory>
#include <iostream>
#include <string>

#include <utils.hpp>
#include <SFML/Graphics.hpp>
#include <UserInterface.hpp>

namespace syxd {

  class Window{
    private:
      std::shared_ptr<sf::RenderWindow> m_MAIN_WINDOW;
      sf::View m_main_view;
      UserInterface m_main_ui;
      std::string m_window_name;
      WINDOW_SETTINGS m_window_settings;
      UI_SETTINGS m_ui_settings;
      sf::Font m_default_font;
      sf::Vector2f half_size;
      
      
      sf::Clock clock;
      sf::Color m_background_color = sf::Color::Black;

    public: 
      sf::Event e_event;

      Window( WINDOW_SETTINGS window_settings );
      ~Window(); 

      std::shared_ptr<sf::RenderWindow> getRenderWindow();
      void setRenderWindow( std::shared_ptr<sf::RenderWindow> window );

      sf::View getMainView();
      void setMainView( sf::View updated_view );

      UserInterface& getUserInterface();
      void setUserInterface( UserInterface updated_UI );

      void EventManager( const float& delta_time );

      void WindowLoop( );

  };

}