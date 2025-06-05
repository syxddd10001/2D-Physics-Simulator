//standard lib

// internal and external headers
#include <SFML/Graphics.hpp>
#include <Circle.hpp>
#include <PhysicsMath.hpp>
#include <Engine.hpp>
#include <DiagnosticInfo.hpp>


using namespace syxd;

/* seperate threads !! not implemented !! */
void EventThread( std::mutex& mtx, std::condition_variable& cv, bool& ready );
void CollisionCheckerThread( std::mutex& mtx, std::condition_variable& cv, bool& ready );
void RenderThread( std::mutex& mtx, std::condition_variable& cv, bool& ready );
void UIThread( std::mutex& mtx, std::condition_variable& cv, bool& ready );

int main() {
  
  Engine Instance( static_cast<uint16_t>(15000) ); // stack allocation for Engine Instance with size
  Instance.MainLoop();
  std::cout<<"exited\n";
  
  return EXIT_SUCCESS;

}