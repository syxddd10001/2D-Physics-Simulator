//standard lib
#include <iostream>
#include <vector>
#include <utility>
#include <thread>
#include <iostream>
#include <memory>

// internal and external headers
#include <SFML/Graphics.hpp>
#include <Circle.hpp>
#include <PhysicsMath.hpp>
#include <Engine.hpp>
//

int main() {
  Engine Instance; // stack allocation for Engine Instance 

  std::chrono::high_resolution_clock::time_point start;

  while ( Instance.WINDOW->isOpen() ) { // program loop
    start = std::chrono::high_resolution_clock::now();
    
    Instance.WINDOW->clear( );
    Instance.m_mouse_pos_f = Instance.WINDOW->mapPixelToCoords( sf::Mouse::getPosition( *(Instance.WINDOW) ) ); // current mouse pos in float
    Instance.m_mouse_pos_i = { static_cast<int>( Instance.m_mouse_pos_f.x ), static_cast<int>( Instance.m_mouse_pos_f.y ) };  // current mouse pos in int
    
    float delta_time = Instance.clock.restart().asSeconds(); // getting deltaTime
    Instance.m_elapsed_time_spawn += delta_time;
    Instance.m_elapsed_time_move += delta_time; 
    Instance.m_elapsed_time_input += delta_time;
    Instance.m_elapsed_time_cursor_blink += delta_time;

    Instance.EventManager( ); // calling event manager ie input
    Instance.CollisionCheck( ); // calling collision checker
    Instance.Update( &delta_time ); // updating and rerendering the positions of objects
    Instance.Render( ); // rerendering any non-UI and non-world elements

    Instance.WINDOW->setView( Instance.m_ui_view ); // setting view for UI, so that UI does not change size when moving / zooming in world
    Instance.UI( ); // rendering UI
    Instance.displayFramesPerSecond( start );
    Instance.WINDOW->setView( Instance.m_main_view ); // resetting view to main 
    Instance.WINDOW->display( ); // displaying everything
  }

  std::cout<<"exited\n";
  return EXIT_SUCCESS;
}