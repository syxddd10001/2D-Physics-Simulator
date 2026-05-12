#pragma once
#include <memory>
#include <iostream>
#include <string>
#include <functional>
#include <vector>

#include <utils.hpp>
#include <SFML/Graphics.hpp>
#include <UserInterface.hpp>

namespace syxd {

  class Window{
    private:
      /*********************** WINDOW VARIABLES ***************/
      std::shared_ptr<sf::RenderWindow> m_MAIN_WINDOW;
      sf::View m_main_view;
      sf::View m_ui_view;
      std::shared_ptr<UserInterface> m_main_ui;
      std::string m_window_name;
      WINDOW_SETTINGS m_window_settings;
      UI_SETTINGS m_ui_settings;
      sf::Font m_default_font;
      sf::Vector2f half_size;
      sf::Clock clock;
      sf::Event e_event;
      sf::Color m_background_color = sf::Color::Black;
      /*****************************************************/

      /*********************** VARIABLE FUNCTIONS ***************************/
      const size_t DEFAULT_NUM_FUNCTIONS = 12;
      std::function<UserInterface()> InitializeUI; // all windows must have a UI initialization function
      std::vector<std::function<void()>> functions; // windows may have external functions that execute 1 by 1 during the main loop of the window 
      std::vector<std::function<void(const sf::Event&, float, std::shared_ptr<sf::RenderWindow>)>> custom_event_handlers; // custom event / input handlers that may be passed on from external classes    
      /*********************************************************************/
      

    public: 
      Window( WINDOW_SETTINGS window_settings, std::shared_ptr<UserInterface> user_interface );
      ~Window();

      Window(const Window&) = delete;
      Window& operator=(const Window&) = delete;

      Window(Window&&) = default;
      Window& operator=(Window&&) = default;

      std::shared_ptr<sf::RenderWindow> getRenderWindow();
      void setRenderWindow( std::shared_ptr<sf::RenderWindow> window );

      sf::View getMainView();
      void setMainView( sf::View updated_view );

      sf::View getUIView();
      void setUIView( sf::View updated_view );

      std::shared_ptr<UserInterface> getUserInterface();
      void setUserInterface( std::shared_ptr<UserInterface> updated_UI );

      void setInitilizeUIFunctionUI( std::function<UserInterface()> updated_function );
      void addFunction( std::function<void()> new_function );

      void EventManager( const float& delta_time );
      void AddEventHandler(std::function<void(const sf::Event&, float, std::shared_ptr<sf::RenderWindow>)> handler);


      void InitializeWindow( WINDOW_SETTINGS window_settings );

      void WindowLoop( );

  };

}