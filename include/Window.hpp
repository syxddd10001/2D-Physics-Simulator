#pragma once

#include <SFML/Graphics.hpp>
#include <UserInterface.hpp>

namespace syxd {

  class Window{
    private:
      sf::RenderWindow m_MAIN_WINDOW;
      sf::View m_main_view;
      UserInterface m_main_ui;

    public: 
      sf::RenderWindow getRenderWindow();
      void setRenderWindow( sf::RenderWindow updated_window );

      sf::View getMainView();
      void setMainView( sf::View updated_view );

      UserInterface getUserInterface();
      void setUserInterface( UserInterface updated_UI );      

  };

}