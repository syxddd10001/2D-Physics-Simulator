#include <Scene.hpp>

Scene::Scene( const WINDOW_SETTINGS& window_settings ) : m_window_settings{window_settings} {
  
  InitializeWindow();
}

void Scene::runScene() {
  if (m_engine_instance == nullptr) {
    std::cout << "No engine was loaded\n";
    return;
  }

  m_engine_instance.get()->MainLoop();
}

void Scene::saveScene( std::string engine_name ) {
  // 1. Get the updated engine data from the running instance
  Engine_Data updated = m_engine_instance->getEngineData();

  // 2. Find the matching engine in m_engines
  auto it = std::find_if(
    m_engines->begin(),
    m_engines->end(),
    [&](const Engine_Data& e) {
      return e.window_settings.WINDOW_NAME == engine_name;
    }
  );

  // 3. Replace the old data with the updated data
  if (it != m_engines->end()) {
    *it = updated;
  } else {
    std::cout << "Engine not found: " << engine_name << "\n";
  }

}

std::vector<Engine_Data> Scene::lazyLoadEnginesFromFile(const std::string& filename) {
    std::ifstream is(filename);
    std::vector<Engine_Data> result;

    if (!is) {
        std::cout << "Couldn't open file\n";
        return result;
    }

    std::string line;

    // optional header
    if (!readCleanLine(is, line))
        return result;

    if (!startsWith(line, "# ENGINE_FILE")) {
        DEBUG_PRINT("Warning: missing # ENGINE_FILE header\n");
    }

    // MAIN LOOP
    while (readCleanLine(is, line)) {
        if (line.empty()) continue;

        // -------------------------
        // ENGINE BLOCK START
        // -------------------------
        if (line == "ENGINE") {
            Engine_Data ed;

            // 1. ENGINE NAME (single quoted line)
            parseEngineName(is, ed);

            // 2. OBJECTS SECTION
            parseObjects(is, ed);

            // 3. WINDOW SETTINGS SECTION
            parseWindowSettings(is, ed);

            result.push_back(std::move(ed));
        }
    }

    return result;
}



bool Scene::loadAllScenes( std::string& scene_path ) {
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

  m_engines.reset();
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

    m_current_scene = target_engine;
    std::cout << m_current_scene->window_settings.DEFAULT_WINDOW_SIZE_X << std::endl;
    std::cout << m_current_scene->window_settings.DEFAULT_WINDOW_SIZE_Y << std::endl;

    m_engine_instance.reset();
    m_engine_instance = std::make_unique<Engine>(*m_current_scene);
    std::function<void()> m_save_function = [this]() {
      this->saveEnginesToFile();
    };

    m_engine_instance->updateSaveFunction( m_save_function );

    return true;
  }

  return false;

}

void Scene::saveEnginesToFile()
{
  if (m_engine_instance) {
    saveScene(m_engine_instance->getEngineData().window_settings.WINDOW_NAME);
  }

  std::ofstream os(m_scenes_path, std::ios::out | std::ios::trunc);
  if (!os) {
    std::cout << "File not found for writing. Not saved.\n";
    return;
  }

  os << "# ENGINE_FILE v1\n";

  // m_engines is a std::unique_ptr<std::vector<Engine_Data>>
  if (!m_engines) return;

  for (const auto& e : *m_engines) {
    os << "ENGINE\n";
    writeQuotedLine(os, e.window_settings.WINDOW_NAME);
    os << "[OBJECTS]\n";
    int id = 1;
    for (const auto& obj : e.p_objects) {
      // Ensure CSV columns: id,type,mass,pos_x,pos_y,dim_x,dim_y,rad,glow,color;
      // Use object's serializeCSV but ensure it ends with a semicolon to match format.
      std::string line = obj->serializeCSV(id);
      line = ensure_trailing_semicolon(std::move(line));
      os << line << "\n";
      ++id;
    }

    os << "[WINDOW_SETTINGS]\n";
    // WINDOW_NAME quoted and line terminated with semicolon
    std::string ws =
        std::to_string(e.window_settings.MAX_FRAME_RATE) + "," +
        std::to_string(e.window_settings.DEFAULT_WINDOW_SIZE_X) + "," +
        std::to_string(e.window_settings.DEFAULT_WINDOW_SIZE_Y) + "," +
        std::to_string(e.window_settings.WORLD_SIZE) + "," +
        "\"" + e.window_settings.WINDOW_NAME + "\"";
    ws = ensure_trailing_semicolon(std::move(ws));
    os << ws << "\n";
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
  m_engine_instance = nullptr;
}

bool Scene::deleteEngine( const std::string engine_name ){
  // find the target engine in m_engines and delete it
  // return true if successfully deleted
  // return false if target engine doesn't exist
  auto& engines = *m_engines;

  for (auto it = engines.begin(); it != engines.end(); ++it) {
    if (it->window_settings.WINDOW_NAME == engine_name) {
      engines.erase(it);
      return true;
    }
  }
  return false;

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
    if (reinitialize){
      InitializeUI();
      loadEnginesToUI();
      reinitialize = false;
    }
    
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
    syxd::UI_Element* elem = (m_user_interface.FindElement("name_input_box"));
    if ( elem ) {
      if ( syxd::InputBox* e = dynamic_cast<syxd::InputBox*>(elem)){  
        if (!e->isFocused()){
          e->setFocused(true);
          e->setOutlineColor(sf::Color::White);
        }
        
        e->checkInput(e_event, MAIN_WINDOW, delta_time);      

      if (e_event.type == sf::Event::KeyPressed && e_event.key.code == sf::Keyboard::Enter){
        std::cout << e->getInputText() << "\n";
        e->clearInput();
      }

      }
    }
    switch( e_event.type ) {  

      case sf::Event::Closed:
        m_is_running = false;
        MAIN_WINDOW->close();
      break;

      case sf::Event::Resized:
        //setZoomLimits ( sf::Vector2f {m_window_settings.WORLD_SIZE, m_window_settings.WORLD_SIZE}, sf::Vector2f(WINDOW->getSize()));
        MAIN_WINDOW->setView( m_ui_view = sf::View( sf::FloatRect( 0.0f, 0.0f, e_event.size.width, e_event.size.height ) ) );
      break;

      case sf::Event::KeyReleased:

        if ( e_event.key.code == sf::Keyboard::F2 ) {     
          
        }
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
  m_text_color = sf::Color::White;

  MAIN_WINDOW->setView( m_ui_view = sf::View( sf::FloatRect( 0.0f, 0.0f, static_cast<float>(m_window_settings.DEFAULT_WINDOW_SIZE_X), 
                                                                  static_cast<float>(m_window_settings.DEFAULT_WINDOW_SIZE_Y) ) ));
  m_user_interface.SetWindow(MAIN_WINDOW);

  InitializeUI();

}

void Scene::InitializeUI( ){
  m_user_interface.RemoveAllElements();
  std::function<void()> show_input;
  std::function<void()> create_new;
  std::function<void()> load_engines;

  show_input = [this](){
    syxd::InputBox* input_box = dynamic_cast<syxd::InputBox*>( m_user_interface.FindElement("name_input_box") );
    syxd::Button* create_button = dynamic_cast<syxd::Button*>( m_user_interface.FindElement("Create") );
    m_show_input = !m_show_input;

    if (create_button != nullptr) {
      create_button->hide( m_show_input );
    }
    if (input_box != nullptr) {
      input_box->hide( m_show_input );
    }

  };

  create_new = [this]() {
    // create a new engine instance
    // put the engine data into m_engines
    // load that engine
    syxd::InputBox* input_box = dynamic_cast<syxd::InputBox*>( m_user_interface.FindElement("name_input_box") );
    if (input_box != nullptr) {
      new_scene_name = input_box->getTextElement().getString();
    }

    if (new_scene_name == "") {
      std::cout << "Type in something in the input box please!\n";
      return;
    }
    Engine_Data* ed = findEngineByName(new_scene_name);
    if (ed != nullptr) 
    {
      std::cout << "Scene already exists\n";
      return;
    }
    
    WINDOW_SETTINGS DEFAULT_WINDOW_SETTINGS = { 144.0f, 1600, 900, 15000, new_scene_name };
    Engine_Data temp_data;
    temp_data.p_objects = {};        
    temp_data.window_settings = DEFAULT_WINDOW_SETTINGS;
    if (m_engines == nullptr){
      m_engines = std::make_unique<vector<Engine_Data>>();
    }
    m_engines->push_back(temp_data);
    m_show_input = !m_show_input;
    reinitialize = true;
    
  };

  load_engines = [this](){ 
    if ( bool loaded = loadAllScenes( m_scenes_path ) ){
      std::cout << "Loaded";
      reinitialize = true;
    }

  };

  m_user_interface.InitText( "scene menu", 
                              "Scene Menu", 
                              m_ui_settings.h1_size, 
                              sf::Vector2f{10,5},
                              m_text_color);


  m_user_interface.InitButton("new_scene_button", 
                              sf::Vector2f{ 100.f, 70.0f },
                              "Create New Scene", 
                              sf::Color::White, // text color
                              sf::Color::Transparent, // background color
                              sf::Color::Green, // hover color
                              sf::Color::Yellow, // outline color
                              sf::Vector2f{150.f, 40.f}, // size
                              {0,0}, //padding
                              true, // hover
                              show_input ); // perform this action on click

 
  m_user_interface.InitInputBox( "name_input_box", m_ui_settings.h2_size, {25.f, 100.0f}, m_text_color );
  syxd::InputBox* input_box = dynamic_cast<syxd::InputBox*>( m_user_interface.FindElement("name_input_box") );
  if (input_box != nullptr) {
    input_box ->setInputBoxSize( {300.0f,25.0f} );
    input_box->setBackgroundColor( sf::Color::Black );
    input_box->setOutlineColor( sf::Color::White );
    input_box->hide(true);
  }


  std::string create_button_name = "Create";
  m_user_interface.InitButton( create_button_name, 
                              sf::Vector2f{ 375.f, 113.0f },
                              create_button_name, 
                              sf::Color::White, // text color
                              sf::Color::Transparent, // background color
                              sf::Color::Green, // hover color
                              sf::Color::Yellow, // outline color
                              sf::Vector2f{ 70.0f, 30.0f }, // size
                              {0,0}, //padding
                              true, // hover
                              create_new ); // perform this action on click
  
  syxd::Button* create_button = dynamic_cast<syxd::Button*>( m_user_interface.FindElement(create_button_name) );

  if (create_button != nullptr) {
    create_button->setCharacterSize(m_ui_settings.h3_size);
    create_button->hide(true);
  }

  std::string load_engines_button_name = "Load Engines";
  m_user_interface.InitButton( load_engines_button_name, 
                              sf::Vector2f{ 100.f, 160.0f },
                              load_engines_button_name, 
                              sf::Color::White, // text color
                              sf::Color::Transparent, // background color
                              sf::Color::Green, // hover color
                              sf::Color::Yellow, // outline color
                              sf::Vector2f{ 150.f, 40.f }, // size
                              {0,0}, //padding
                              true, // hover
                              load_engines ); // perform this action on click
  
  syxd::Button* load_button = dynamic_cast<syxd::Button*>( m_user_interface.FindElement(load_engines_button_name) );
}

void Scene::loadEnginesToUI(){
  float engine_list_init_position = 220.0f;
  
  if ( m_engines != nullptr ){
    for (int i = 0; i < m_engines->size(); i++){
      float offset = 50.f;
      float y_position = engine_list_init_position+offset*(i);
      std::string button_name = m_engines->at(i).window_settings.WINDOW_NAME;

      std::string delete_name = "Delete";
      std::function<void()> run_scene;
      std::function<void()> delete_engine;
 
      run_scene = [this, button_name, i]() {
        bool loaded = this->loadScene(button_name);
        if (loaded) {
          std::cout << "Loaded " << button_name << '\n';
          runScene(); // and then run the current scene (ie the engine)
          m_engine_instance.reset();
          reinitialize = true; // reinitializeUI
        }
      };

      delete_engine = [this, button_name, delete_name]() {
        bool deleted = this->deleteEngine(button_name);
        if (deleted){
          std::cout << "Deleted " << button_name << '\n';
          m_engine_instance.reset();
          saveEnginesToFile();
          reinitialize = true; // reinitializeUI
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
                                  run_scene ); // perform this action on click 
        
      
      m_user_interface.InitButton(delete_name + button_name, 
                                  sf::Vector2f{ 230.f, y_position },
                                  delete_name, 
                                  sf::Color::White, // text color
                                  sf::Color::Transparent, // background color
                                  sf::Color::Red, // hover color
                                  sf::Color::Red, // outline color
                                  sf::Vector2f{70.f, 40.f}, // size
                                  {0,0}, //padding
                                  true, // hover
                                  delete_engine ); // perform this action on click 



    }
    
  } else std::cout << "No engines are loaded, nothing to show\n";

}