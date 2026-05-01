#include <Scene.hpp>

Scene::Scene( const WINDOW_SETTINGS& window_settings ) : m_window_settings{window_settings} {

  if ( bool loaded = loadAllScenes( m_scenes_path ) ){
    std::cout << "Loaded";
  }
  
  InitializeWindow();
}

void Scene::runScene(){
  m_engine_instance.get()->MainLoop();
}

bool Scene::loadAllScenes( std::string& scene_path ){
  // read a file 
  // expand the lines
  // get data from fields
  // parse data into variables
  // put data into appropriate variables in the engine

  WINDOW_SETTINGS window_settings;

  std::vector<Engine_Data> engines = loadEnginesFromFile( scene_path );

  if (engines.size() == 0){
    return false;
  }

  m_engines = std::make_unique<vector<Engine_Data>>(engines);

  if (m_engines) return true;

  return false;
}

bool Scene::loadScene( std::string engine_name ) {
  if (!m_engines) return false;

  std::cout << "loading " << engine_name << '\n' ;

  Engine_Data* target_engine = findEngineByName( engine_name );

  if (target_engine != nullptr) {
    std::cout << "target engine found...\n";

    m_current_scene = *target_engine;
    std::cout << m_current_scene.window_settings.DEFAULT_WINDOW_SIZE_X << std::endl;
    std::cout << m_current_scene.window_settings.DEFAULT_WINDOW_SIZE_Y << std::endl;

    m_engine_instance = nullptr;
    m_engine_instance = std::make_unique<Engine>(m_current_scene);

    return true;
  }

  return false;

}

bool Scene::saveScene( std::string& scene_path ){
  // save logic
  return false;
}

void saveEnginesToFile(const string& filename, const vector<Engine_Data>& engines) {
  std::ofstream os(filename, std::ios::out);
  if (!os) {
    throw std::runtime_error("Cannot open file for writing");
  }
  os << "# ENGINE_FILE v1\n";
  for (const auto& e : engines) {
    os << "ENGINE\n";
    writeQuotedLine(os, e.window_settings.WINDOW_NAME);
    os << "[OBJECTS]\n";
    int id = 1;
    for (const auto& obj : e.p_objects) {
      // Ensure CSV columns: id,type,mass,pos_x,pos_y,dim_x,dim_y,rad,glow;
      // Use object's serializeCSV but ensure Circle/Rectangle produce correct column order.
      string line = obj->serializeCSV(id);
      os << line << "\n";
      ++id;
    }
    os << "[WINDOW_SETTINGS]\n";
    // WINDOW_NAME quoted
    os << e.window_settings.MAX_FRAME_RATE << ","
       << e.window_settings.DEFAULT_WINDOW_SIZE_X << ","
       << e.window_settings.DEFAULT_WINDOW_SIZE_Y << ","
       << e.window_settings.WORLD_SIZE << ","
       << '"' << e.window_settings.WINDOW_NAME << '"' << ";\n";
  }
}

std::vector<Engine_Data> Scene::loadEnginesFromFile( const std::string& filename ) {
  std::ifstream is(filename);
  std::vector<Engine_Data> result;

  if (!is) {
    std::cout << "Couldn't open file\n";
    return result;
  }

  std::string line;

  // optional header
  if (!std::getline(is, line))
    return result;

  if (!startsWith(line, "# ENGINE_FILE")) {
      // warn but continue if you want
      // std::cout << "Warning: missing # ENGINE_FILE header\n";
  }

  while (std::getline(is, line)) {
    line = cleanLine(line);
    if (line.empty()) continue;

    if (line == "ENGINE") {
      Engine_Data ed;

      // ENGINE name
      parseEngineName(is, ed);

      // OBJECTS + WINDOW_SETTINGS
      parseObjects(is, ed);
      parseWindowSettings(is, ed);

      result.push_back(std::move(ed));
    }
  }
  return result;
}

const std::string Scene::getScenesPath( ){
  return m_scenes_path;
}

void Scene::setScenesPath( const std::string& new_path ){
  m_scenes_path = new_path;
}

const std::vector<Engine_Data>* Scene::getAllEngines() const {
  return m_engines.get(); // ensure m_engines != nullptr
}

void Scene::addEngine( Engine_Data engine_data ){
  m_engines.get()->push_back(engine_data);
}
void Scene::clearAllEngines( ){
  if (m_engines) m_engines.get()->clear();
}

Engine_Data* Scene::findEngineByName( const std::string& target ) const {
  const std::vector<Engine_Data>* engines = getAllEngines(); 
  
  if (!engines) return nullptr;

  for (const auto& eg : *engines) { 
    if (eg.window_settings.WINDOW_NAME == target ) {
      return const_cast<Engine_Data*>(&eg); 
    }
  }

  return nullptr;
}

void Scene::DisplayMenu( ){
  m_is_running = true;
  while ( m_is_running && MAIN_WINDOW != nullptr) { 
    if ( MAIN_WINDOW->isOpen() ) {
      MAIN_WINDOW->clear(m_background_color );
    }
  
    float delta_time = clock.restart().asSeconds();
    EventManager( delta_time );

    m_user_interface.RenderUI( delta_time );
    
    if ( MAIN_WINDOW->isOpen() ) {
      MAIN_WINDOW->display();
    }
  }
}

void Scene::EventManager( const float& delta_time ){
  if( MAIN_WINDOW != nullptr && MAIN_WINDOW->pollEvent( e_event ) ) {
    switch( e_event.type ) {
      case sf::Event::Closed:
        m_is_running = false;
        MAIN_WINDOW->close();
      break;

      case sf::Event::Resized:
        //setZoomLimits ( sf::Vector2f {m_window_settings.WORLD_SIZE, m_window_settings.WORLD_SIZE}, sf::Vector2f(WINDOW->getSize()));
        MAIN_WINDOW->setView( m_ui_view = sf::View( sf::FloatRect( 0.0f, 0.0f, e_event.size.width, e_event.size.height ) ) );
      break;
    }
  }
}

void Scene::InitializeWindow(){
  MAIN_WINDOW = std::make_shared<sf::RenderWindow>( sf::VideoMode( m_window_settings.DEFAULT_WINDOW_SIZE_X, 
                                                              m_window_settings.DEFAULT_WINDOW_SIZE_Y ),
                                                              m_window_settings.WINDOW_NAME );
  
  m_ui_view = sf::View( sf::FloatRect( 0, 0, MAIN_WINDOW->getSize().x, MAIN_WINDOW->getSize().y ) );
  MAIN_WINDOW->setFramerateLimit( m_window_settings.MAX_FRAME_RATE );
  MAIN_WINDOW->setView( m_ui_view = sf::View( sf::FloatRect( 0.0f, 0.0f, static_cast<float>(m_window_settings.DEFAULT_WINDOW_SIZE_X), 
                                                                  static_cast<float>(m_window_settings.DEFAULT_WINDOW_SIZE_Y) ) ));
  
  #if DEBUG
    if ( !m_default_font.loadFromFile( fonts[0] ) ){
        DEBUG_PRINT("Font not found\n");
    }
    else {
        DEBUG_PRINT("Font loaded\n");

    }
    
  #else
    if ( !m_default_font.loadFromFile( "static/fonts/cairo.ttf" ) ){
        DEBUG_PRINT("Font not found\n");
    }
  #endif

  m_user_interface.SetFont( m_default_font );

  
  m_background_color = sf::Color::Black;
  m_text_color = sf::Color::Red;

  MAIN_WINDOW->setView( m_ui_view = sf::View( sf::FloatRect( 0.0f, 0.0f, static_cast<float>(m_window_settings.DEFAULT_WINDOW_SIZE_X), 
                                                                  static_cast<float>(m_window_settings.DEFAULT_WINDOW_SIZE_Y) ) ));
  m_user_interface.SetWindow(MAIN_WINDOW);

  InitializeUI();

}

void Scene::InitializeUI( ){
  m_user_interface.InitText( "scene menu", 
                              "Scene Menu", 
                              m_ui_settings.h3_size, 
                              sf::Vector2f{10,15},
                              m_text_color);

  
  
  std::string button_name = "New Scene";
  std::function<void()> create_new;
  
  create_new = [this, button_name]() {
    
    // create a new engine instance
    // put the engine data into m_engines
    // load that engine

    WINDOW_SETTINGS DEFAULT_WINDOW_SETTINGS = { 144.0f, 1600, 900, 15000, button_name};
    Engine instance(DEFAULT_WINDOW_SETTINGS);
    Engine_Data constructed = instance.getEngineData();

    m_engines->push_back(constructed);
    
    bool loaded = this->loadScene(button_name);
    if (loaded) {
      std::cout << "Loaded " << button_name << '\n';
      m_is_running = false;
      MAIN_WINDOW->close(); // close the menu window 
      runScene(); // and then run the current scene (ie the engine)
    }
  };

  m_user_interface.InitButton(button_name, 
  sf::Vector2f{ 100.f, 70.0f },
  button_name, 
  sf::Color::White, // text color
  sf::Color::Transparent, // background color
  sf::Color::Green, // hover color
  sf::Color::Yellow, // outline color
  sf::Vector2f{150.f, 40.f}, // size
  {0,0}, //padding
  true, // hover
  create_new ); // perform this action on click

  float init_position = 120.0f;

  if ( m_engines != nullptr ){
    for (int i = 0; i < m_engines->size(); i++){
      float offset = 50.f;
      float y_position = init_position+offset*(i);
      std::string button_name = m_engines->at(i).window_settings.WINDOW_NAME;
      std::function<void()> action;
      
      action = [this, button_name]() {
        bool loaded = this->loadScene(button_name);
        if (loaded) {
          std::cout << "Loaded " << button_name << '\n';
          //m_is_running = false;
          //MAIN_WINDOW->close(); // close the menu window 
          runScene(); // and then run the current scene (ie the engine)
        }
      };
      m_user_interface.InitButton(button_name, 
      sf::Vector2f{ 100.f, y_position },
      button_name, 
      sf::Color::White, // text color
      sf::Color::Transparent, // background color
      sf::Color::Green, // hover color
      sf::Color::Yellow, // outline color
      sf::Vector2f{150.f, 40.f}, // size
      {0,0}, //padding
      true, // hover
      action ); // perform this action on click 
    }
    
  } else std::cout << "sadly engines is nullptr :(\n";
  

   m_user_interface.InitButton("button 1", 
    sf::Vector2f{100,500}, 
    "Save Test", 
    sf::Color::White, // text color
    sf::Color::Transparent, // background color
    sf::Color::Green, // hover color
    sf::Color::Yellow, // outline color
    sf::Vector2f{100.f, 40.f}, // size
    {0,0}, //padding
    true, // hover
    NULL ); // perform this action on click

}

