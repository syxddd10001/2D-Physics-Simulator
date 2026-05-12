//standard lib
#include <iostream>
#include <string>
#include <functional>

// internal and external headers
#include <SFML/Graphics.hpp>
#include <Circle.hpp>
#include <PhysicsMath.hpp>
#include <Engine.hpp>
#include <DiagnosticInfo.hpp>
#include <Scene.hpp>
#include <Window.hpp>

using namespace syxd;

/* seperate threads !! not implemented !! */
void EventThread( std::mutex& mtx, std::condition_variable& cv, bool& ready );
void CollisionCheckerThread( std::mutex& mtx, std::condition_variable& cv, bool& ready );
void RenderThread( std::mutex& mtx, std::condition_variable& cv, bool& ready );
void UIThread( std::mutex& mtx, std::condition_variable& cv, bool& ready );

Window testWindow(){
  const float MAX_FRAME_RATE = 144.0f; // max frame rate of this window
  const uint16_t DEFAULT_WINDOW_SIZE_X = 1500; // 
  const uint16_t DEFAULT_WINDOW_SIZE_Y = 900;
  const uint16_t WORLD_SIZE = 15000; // always square
  const std::string WINDOW_NAME = "Test Window";
  WINDOW_SETTINGS window_settings = { MAX_FRAME_RATE, DEFAULT_WINDOW_SIZE_X, DEFAULT_WINDOW_SIZE_Y, WORLD_SIZE, WINDOW_NAME };
  


  std::function<UserInterface()> init_ui_function = []() {
    UserInterface ui;
  
    ui.InitText( "scene menu", 
                  "Scene Menu", 
                  25, 
                  sf::Vector2f{10,5},
                  sf::Color::Red );
    return ui;
  };
  
  std::function<void()> test_function = []() {
    
  };
  

  Window window(window_settings, init_ui_function ); 
  window.addFunction(test_function);

  return window;
}


int main() {
  const float MAX_FRAME_RATE = 144.0f; // max frame rate of this window
  const uint16_t DEFAULT_WINDOW_SIZE_X = 1500; // 
  const uint16_t DEFAULT_WINDOW_SIZE_Y = 900;
  const uint16_t WORLD_SIZE = 15000; // always square
  const std::string WINDOW_NAME = "Scene Menu";


  WINDOW_SETTINGS window_settings = { MAX_FRAME_RATE, DEFAULT_WINDOW_SIZE_X, DEFAULT_WINDOW_SIZE_Y, WORLD_SIZE, WINDOW_NAME };

  if (1 == 0) {
  Scene scene1( window_settings );

    scene1.DisplayMenu();
    WINDOW_SETTINGS settings = scene1.m_engine_instance->getWindowSetting();
    DEBUG_PRINT("window settings: %f, %d, %d, %d, %s\n", 
          settings.MAX_FRAME_RATE, 
          settings.DEFAULT_WINDOW_SIZE_X, 
          settings.DEFAULT_WINDOW_SIZE_Y, 
          settings.WORLD_SIZE, 
          settings.WINDOW_NAME.c_str() );

  } else {
    auto test_window(testWindow());
    test_window.WindowLoop();
  }

  DEBUG_PRINT("exited\n");
  
  return EXIT_SUCCESS;

}