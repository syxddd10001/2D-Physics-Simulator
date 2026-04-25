#include "Engine.hpp"
#include <vector>
#include <memory>
#include <iostream>
#include <string>
#include <fstream>


namespace syxd {
  using namespace syxd;
  
  class Scene{
    private:
    Engine_Data m_current_scene;
    std::string m_scenes_path = "data/engines.dtf";
    std::unique_ptr<std::vector<Engine_Data>> m_engines;

    public:
    std::unique_ptr<Engine> m_engine_instance;
    
    Scene ( std::unique_ptr<Engine> instance ) {
      instance.swap(m_engine_instance); 
    }
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


  };  

}