#pragma once
#include <vector>
#include <memory>
#include <iostream>
#include <string>
#include <fstream>
#include <time.h>

#include "Engine.hpp"

#include <SFML/Graphics.hpp>
#include <SFML/Window/Clipboard.hpp>


namespace syxd {
  using namespace syxd;
  
  class Scene{
    private:
    Engine_Data m_current_scene;
    std::string m_scenes_path = "data/engines.dtf";
    std::unique_ptr<std::vector<Engine_Data>> m_engines;
    shared_ptr<sf::RenderWindow> MAIN_WINDOW;
    sf::Font m_default_font;
    sf::Color m_background_color;
    sf::Color m_text_color;
    sf::Clock clock;

    const std::string FONT_PATH = "static/fonts/";
    const std::string fonts[5] =  
    { 
      FONT_PATH + "cairo.ttf",
      FONT_PATH + "FiraCode.ttf",
      FONT_PATH + "Silver.ttf",
      FONT_PATH + "simsun.ttf"
    };

    WINDOW_SETTINGS m_window_settings; // window/world settings
    UI_SETTINGS m_ui_settings = { 30, 20, 15, 12 }; // ui settings

    sf::View m_ui_view;
    UserInterface m_user_interface; // scenes's user interface
    sf::Event e_event;
    std::chrono::high_resolution_clock::time_point start;

    bool m_is_running = false; // sad

    public:
    std::unique_ptr<Engine> m_engine_instance;
    
    Scene ( const WINDOW_SETTINGS& window_settings );
    
      //instance.swap(m_engine_instance); 
    
    ~Scene (){ };

    void runScene();

    bool loadAllScenes( std::string& scene_path );
    
    bool loadScene( std::string& engine_name );

    bool saveScene( std::string& scene_path );

    vector<Engine_Data> loadEnginesFromFile( const std::string& filename );
    
    const std::string getScenesPath( );
    void setScenesPath( const std::string& new_path ); 

    const std::vector<Engine_Data>* getAllEngines() const;
    void addEngine( Engine_Data engine_data );
    void clearAllEngines( );

    Engine_Data* findEngineByName( const std::string& target ) const;

    void DisplayMenu();
    void EventManager( const float& delta_time );
    void InitializeUI( );
    void InitializeWindow( );


  };  

}