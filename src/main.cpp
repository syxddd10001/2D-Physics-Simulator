//standard lib
#include <conio.h>
// internal and external headers
#include <SFML/Graphics.hpp>
#include <Circle.hpp>
#include <PhysicsMath.hpp>
#include <Engine.hpp>
#include <DiagnosticInfo.hpp>

void EventThread( std::mutex& mtx, std::condition_variable& cv, bool& ready );
void CollisionCheckerThread( std::mutex& mtx, std::condition_variable& cv, bool& ready );
void RenderThread( std::mutex& mtx, std::condition_variable& cv, bool& ready );

void UIThread( std::mutex& mtx, std::condition_variable& cv, bool& ready );

int main() {
  Engine Instance( static_cast<uint16_t>(10000) ); // stack allocation for Engine Instance 
  std::chrono::high_resolution_clock::time_point start;
  init();
  while ( Instance.isRunning ) {
    start = std::chrono::high_resolution_clock::now(); // for benchmarking
    Instance.WINDOW->clear( );
    Instance.m_mouse_pos_f = Instance.WINDOW->mapPixelToCoords( sf::Mouse::getPosition( *(Instance.WINDOW) ) ); // current mouse pos in float
    Instance.m_mouse_pos_i = { static_cast<int>( Instance.m_mouse_pos_f.x ), static_cast<int>( Instance.m_mouse_pos_f.y ) };  // current mouse pos in int
    float delta_time = Instance.clock.restart().asSeconds(); // getting deltaTime
    
    Instance.EventManager( delta_time ); // calling event manager ie to handle inputs
    Instance.CollisionCheck( ); // calling collision checker
    Instance.UpdatePhysics( delta_time ); // updating and rerendering the positions of objects
    Instance.Render( ); // rendering any non-UI and non-world elements
    Instance.WINDOW->setView( Instance.m_ui_view ); // setting view for UI, so that UI does not change size when moving / zooming in world
    Instance.UI( ); // rendering UI
    
    // diagnostic info for benchmarking
    const uint64_t cpu_usage = GetCPUUsage();
    const uint64_t memory_available = GetMemoryUsage().available;
    const uint64_t memory_used = GetMemoryUsage().used;
    Instance.displayDiagnosticInfo( start, cpu_usage, memory_available, memory_used );  
    Instance.WINDOW->setView( Instance.m_main_view ); // resetting view to main 
    Instance.WINDOW->display(); 
  }
  std::cout<<"exited\n";
  return EXIT_SUCCESS;
}