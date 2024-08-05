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


int main(){
  std::unique_ptr<Engine> Instance = std::make_unique<Engine>( ); // unique pointer that reserves memory in heap for an Engine instance 

  std::chrono::high_resolution_clock::time_point start;

  while ( Instance->WINDOW->isOpen() ) { // program loop
    start = std::chrono::high_resolution_clock::now();
    
    Instance->WINDOW->clear( );

    Instance->mousePosf = Instance->WINDOW->mapPixelToCoords(sf::Mouse::getPosition( *(Instance->WINDOW) ) ); // current mouse pos in float
    Instance->mousePos = { static_cast<int>( Instance->mousePosf.x ), static_cast<int>( Instance-> mousePosf.y ) };  // current mouse pos in int
    
    
    float delta_time = Instance->clock.restart().asSeconds(); // getting deltaTime
    Instance->elapsed_time_spawn += delta_time;
    Instance->elapsed_time_move += delta_time; 
    Instance->elapsed_time_input += delta_time;
    Instance->elapsed_time_cursor_blink += delta_time;

    Instance->EventManager( ); // calling event manager ie input
    
    Instance->collisionCheck( ); // calling collision checker

    Instance->Update( &delta_time ); // updating and rerendering the positions of objects

    Instance->Render( ); // rerendering any non-UI and non-world elements

    Instance->WINDOW->setView( Instance->UIView ); // setting view for UI, so that UI does not change size when moving / zooming in world
    Instance->UI( ); // rendering UI
    Instance->displayFramesPerSecond( start );
    Instance->WINDOW->setView( Instance->mainView ); // resetting view to main
    Instance->WINDOW->display( ); // displaying everything

  }

  std::cout<<"exited\n";
  return EXIT_SUCCESS;
}