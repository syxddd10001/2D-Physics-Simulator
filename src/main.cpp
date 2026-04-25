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
  const std::string WINDOW_NAME;

  WINDOW_SETTINGS window_settings = { MAX_FRAME_RATE, DEFAULT_WINDOW_SIZE_X, DEFAULT_WINDOW_SIZE_Y, WORLD_SIZE, WINDOW_NAME };

  std::unique_ptr<Engine> instance = std::make_unique<Engine>(window_settings);
  Scene scene1( std::move(instance) );
  std::string scene_path = scene1.getScenesPath();
  
  bool loaded = scene1.loadAllScenes( scene_path );
  if (loaded) {
    std::string scene = "First Engine";
    bool sceneLoaded = scene1.loadScene(scene);
    if (sceneLoaded){
      std::cout<<"Scene Loaded\n";
      scene1.runScene();
    }
    else {
      std::cout<<"Scene could not be loaded\n";
      
    }
  }
  else {
    std::cout<<"No scenes saved or file not found\n";
    scene1.runScene();
  }
  

  //scene1.runScene();

  std::cout<<"exited\n";
  
  return EXIT_SUCCESS;

}