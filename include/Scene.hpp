#pragma once
#include <vector>
#include <memory>
#include <iostream>
#include <string>
#include <fstream>
#include <time.h>
#include <unordered_map>
#include <unordered_set>

#include "Engine.hpp"
#include "Parser.hpp"

#include <SFML/Graphics.hpp>
#include <SFML/Window/Clipboard.hpp>

const std::string DTF_VERSION = "2";

namespace syxd {
  using namespace syxd;
  
  class Scene{
    private:
    Engine_Data* m_current_scene;
    std::string m_scenes_path = "data/engines.dtf";
    std::string new_scene_name = "";
    std::vector<Engine_Block> m_engines;
    shared_ptr<sf::RenderWindow> MAIN_WINDOW;
    sf::Font m_default_font;
    sf::Color m_background_color;
    sf::Color m_text_color;
    sf::Clock clock;
    bool reinitialize = false;

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
    bool m_show_input = true;

    public:
    std::unique_ptr<Engine> m_engine_instance;
    
    Scene ( const WINDOW_SETTINGS& window_settings );
    ~Scene (){ };

    void runScene();

    
    bool lazyLoadEnginesFromFile( const std::string& filename ); // loads partial engine data from file (no objects)
    bool loadAllScenes( std::string& scene_path );
    bool loadScene( std::string engine_name );
    void saveScene( std::string engine_name );
    void saveEnginesToFile(  );
    
    bool writeEngineDataToFile( std::string file_name, std::vector<Engine_Data> engine_data );

    vector<Engine_Data> loadEnginesFromFile( const std::string& filename );
    void loadEnginesToUI();
    
    const std::string getScenesPath( );
    void setScenesPath( const std::string& new_path ); 

    const std::vector<Engine_Block> getAllEngineBlocks() const;

    void addEngine( Engine_Block engine_block );

    void clearAllEngines( );

    Engine_Block findEngineByNameFromBlock( const std::string& target ) const;

    void DisplayMenu();
    void EventManager( const float& delta_time );
    void InitializeUI( );
    void InitializeWindow( );
    bool deleteEngine( const std::string engine_name );

    std::vector<Engine_Block> getEngineBlocksFromFile( const std::string& filename ); // reads file and puts it in a Engine_Block data structure in a raw format without parsing them into actual engine objects
  };  

}