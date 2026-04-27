//standard lib

// internal and external headers
#include <SFML/Graphics.hpp>
#include <Circle.hpp>
#include <PhysicsMath.hpp>
#include <Engine.hpp>
#include <DiagnosticInfo.hpp>
#include <Scene.hpp>

using namespace syxd;

/* seperate threads !! not implemented !! */
void EventThread( std::mutex& mtx, std::condition_variable& cv, bool& ready );
void CollisionCheckerThread( std::mutex& mtx, std::condition_variable& cv, bool& ready );
void RenderThread( std::mutex& mtx, std::condition_variable& cv, bool& ready );
void UIThread( std::mutex& mtx, std::condition_variable& cv, bool& ready );

int main() {
  const float MAX_FRAME_RATE = 144.0f; // max frame rate of this window
  const uint16_t DEFAULT_WINDOW_SIZE_X = 1500; // 
  const uint16_t DEFAULT_WINDOW_SIZE_Y = 900;
  const uint16_t WORLD_SIZE = 15000; // always square
  const std::string WINDOW_NAME = "Scene Menu";
  std::string ENGINE_NAME = "First Engine";

  WINDOW_SETTINGS window_settings = { MAX_FRAME_RATE, DEFAULT_WINDOW_SIZE_X, DEFAULT_WINDOW_SIZE_Y, WORLD_SIZE, WINDOW_NAME };

  Scene scene1( window_settings );
  scene1.loadScene(ENGINE_NAME);

  //scene1.DisplayMenu();
  WINDOW_SETTINGS settings = scene1.m_engine_instance->getWindowSetting();
  std::cout << "window name: " << settings.WINDOW_NAME << "\n";
  scene1.runScene();
  

  std::cout<<"exited\n";
  
  return EXIT_SUCCESS;

}