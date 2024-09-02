//standard lib

// internal and external headers
#include <SFML/Graphics.hpp>
#include <Circle.hpp>
#include <PhysicsMath.hpp>
#include <Engine.hpp>
#include <DiagnosticInfo.hpp>

using namespace syxd;

void EventThread( std::mutex& mtx, std::condition_variable& cv, bool& ready );
void CollisionCheckerThread( std::mutex& mtx, std::condition_variable& cv, bool& ready );
void RenderThread( std::mutex& mtx, std::condition_variable& cv, bool& ready );

void UIThread( std::mutex& mtx, std::condition_variable& cv, bool& ready );

int main() {
  Engine Instance( static_cast<uint16_t>(10000) ); // stack allocation for Engine Instance 
  Instance.MainLoop();
  std::cout<<"exited\n";
  return EXIT_SUCCESS;
}