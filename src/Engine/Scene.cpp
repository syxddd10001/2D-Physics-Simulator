#include <Scene.hpp>

Scene::Scene( const WINDOW_SETTINGS& window_settings ) : m_window_settings{window_settings} {
  
  InitializeWindow();
}

void Scene::runScene() {
  if (m_engine_instance == nullptr) {
    DEBUG_PRINT("No engine instance found. Cannot run scene.\n");
    return;
  }

  m_engine_instance.get()->MainLoop();
}


void Scene::saveScene( std::string engine_name ) {
  // 1. Get the updated engine data from the running instance
  Engine_Data updated = m_engine_instance->getEngineData();
  Engine_Block updatedBlock = engineDataToBlock(updated);
  // 2. Find the matching engine in engine vector
  auto it = std::find_if(
    m_engines.begin(),
    m_engines.end(),
    [&](const Engine_Block& e) {
      return e.engine_name == engine_name;
    }
  );

  // 3. Replace the old data with the updated data
  if (it != m_engines.end()) {
    *it = updatedBlock;
  } else {
    DEBUG_PRINT("Engine not found: %s\n", engine_name.c_str());
  }
}

void saveScene( std::vector<Engine_Data> engines, std::unique_ptr<Engine> target_engine ) {
  // 1. Get the updated engine data from the target engine
  Engine_Data updated = target_engine->getEngineData();
  std::string engine_name = updated.window_settings.WINDOW_NAME;

  // 2. Find the matching engine in engine vector
  auto it = std::find_if(
    engines.begin(),
    engines.end(),
    [&](const Engine_Data& e) {
      return e.window_settings.WINDOW_NAME == engine_name;
    }
  );

  // 3. Replace the old data with the updated data
  if (it != engines.end()) {
    *it = updated;
  } else {
    DEBUG_PRINT("Engine not found: %s\n", engine_name.c_str());
  }

}

std::vector<Engine_Block> Scene::getEngineBlocksFromFile( const std::string& filename ) {
    std::ifstream is(filename);

    if (!is) {
        DEBUG_PRINT("Couldn't open file\n");
        return {};
    }

    std::string line;

    // optional header
    if (!readCleanLine(is, line))
        return {};

    if (!startsWith(line, "# ENGINE_FILE")) {
        DEBUG_PRINT("Warning: missing # ENGINE_FILE header\n");
    }

    std::vector<Engine_Block> current = parseEngines(is);

    return current;
}

bool Scene::lazyLoadEnginesFromFile( const std::string& filename ) {

  /****************************************************
    LOAD FILE AND STORE IT IN RAW ENGINE BLOCK FORMAT WITHOUT PARSING IT INTO ENIGNE OBJECTS 
  ****************************************************/
  std::vector<Engine_Block> current = getEngineBlocksFromFile( filename );
  for (auto c : current){
    for (auto o : c.objects) DEBUG_PRINT("%s\n", o.c_str());
    DEBUG_PRINT("%s\n", c.window_settings.c_str());
  }

  m_engines = current;
  return true;

}

bool Scene::loadAllScenes( std::string& scene_path ) {
  // read a file 
  // expand the lines
  // get data from fields
  // parse data into variables
  // put data into appropriate variables in the engine

  WINDOW_SETTINGS window_settings;

  //std::vector<Engine_Data> engines = lazyLoadEnginesFromFile( scene_path );
  std::vector<Engine_Block> engines = getEngineBlocksFromFile( scene_path );
  m_engines = engines;

  if (engines.size() == 0){
    return false;
  } else 
    return true;

}

bool Scene::loadScene( std::string engine_name ) {
  if (m_engines.size() == 0) return false;

  DEBUG_PRINT("loading %s\n", engine_name.c_str());
  Engine_Block target_engine = { "", {}, ""};
  target_engine = findEngineByNameFromBlock( engine_name );

  if ( target_engine.engine_name != "" ) {
    DEBUG_PRINT("target engine found...\n");

    m_current_scene = new Engine_Data(parseEngineBlock( target_engine ));
    DEBUG_PRINT("Default window size X: %d\n", m_current_scene->window_settings.DEFAULT_WINDOW_SIZE_X);
    DEBUG_PRINT("Default window size Y: %d\n", m_current_scene->window_settings.DEFAULT_WINDOW_SIZE_Y);

    m_engine_instance.reset();
    m_engine_instance = std::make_unique<Engine>(*m_current_scene);
    std::function<void()> m_save_function = [this]() {
      this->saveEnginesToFile();
    };

    m_engine_instance->updateSaveFunction( m_save_function );

    return true;
  } else DEBUG_PRINT("engine %s not found!\n", engine_name.c_str());

  return false;
}

void Scene::saveEnginesToFile()
{
  /***************SAVING LOCALLY****************/
  if (m_engine_instance) {
    bool load_result = lazyLoadEnginesFromFile( m_scenes_path );
    if (!load_result){
      DEBUG_PRINT("save failed: failed to load file.\n");
      return;
    }
    std::vector<Engine_Data> engines = parseAllEngineBlocks(m_engines);

    Engine_Data current_engine_data = m_engine_instance->getEngineData(); // get current engine data
    Engine_Block current_engine_block = engineDataToBlock( current_engine_data ); // get current engine block from file (to ensure we have the latest block data structure)

    if (findEngineByNameFromBlock(current_engine_data.window_settings.WINDOW_NAME).engine_name == "") {
      // if engine doesnt exist (meaning newly created), push it to all engines vector
      engines.push_back(current_engine_data);
      m_engines.push_back(current_engine_block);
      DEBUG_PRINT("Engine pushed\n");
    }

    saveScene( current_engine_block.engine_name ); // at last, save scene
    engines = parseAllEngineBlocks(m_engines);

    writeEngineDataToFile( m_scenes_path, engines );
  }
  /*******************************************/

}

bool Scene::writeEngineDataToFile( std::string file_name, std::vector<Engine_Data> engine_data ){
    /*********************** WRITING TO FILE **************************/
  std::ofstream os(file_name, std::ios::out | std::ios::trunc);
  if (!os) {
    DEBUG_PRINT("File not found for writing. Not saved.\n");
    return false;
  }

  os << "# ENGINE_FILE v" << DTF_VERSION << "\n";

  // nothing to write
  if (engine_data.size() == 0) return false;

  for (const auto& e : engine_data) {
    os << "[ENGINE]\n";
    writeQuotedLine(os, e.window_settings.WINDOW_NAME);
    os << "[OBJECTS]\n";
    int id = 1;
    int ID =1;

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

  /*********************************************************************/

  }
  return true;
}


std::vector<Engine_Data> Scene::loadEnginesFromFile( const std::string& filename ) {
  std::ifstream is(filename);
  std::vector<Engine_Data> result;

  if (!is) {
    DEBUG_PRINT("Couldn't open file\n");
    return result;
  }

  std::string line;

  // optional header
  if (!std::getline(is, line))
    return result;

  if (!startsWith(line, "# ENGINE_FILE")) {
    // warn but continue if you want
    // DEBUG_PRINT("Warning: missing # ENGINE_FILE header\n");
  }

  while (std::getline(is, line)) {
    line = cleanLine(line);
    if (line.empty()) continue;

    if (line == "[ENGINE]") {
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

const std::vector<Engine_Block> Scene::getAllEngineBlocks() const {
  return m_engines;
}


void Scene::addEngine( Engine_Block engine_block ){
  m_engines.push_back(engine_block);
}

void Scene::clearAllEngines( ){
  m_engines.clear();
  m_engine_instance = nullptr;
}

bool Scene::deleteEngine( const std::string engine_name ){
  // find the target engine in engine_vector and delete it
  // return true if successfully deleted
  // return false if target engine doesn't exist

  bool load_result = lazyLoadEnginesFromFile( m_scenes_path );
  if (!load_result){
    DEBUG_PRINT("save failed: failed to load file.\n");
    return false;
  }

  bool deleted = false;

  for (auto it = m_engines.begin(); it != m_engines.end(); ++it) {
    if (it->engine_name == engine_name) {
      m_engines.erase(it);
      deleted = true;
      break;
    }
  }

  if (!deleted) return false;

  std::vector<Engine_Data> engines = parseAllEngineBlocks(m_engines);
 
  writeEngineDataToFile( m_scenes_path, engines );

  return true;
}

Engine_Block Scene::findEngineByNameFromBlock( const std::string& target ) const {
  const std::vector<Engine_Block> engines = getAllEngineBlocks(); 
  
  if (engines.size() == 0) return {};

  for (const auto& eg : engines) { 
    DEBUG_PRINT("Engine name: %s\n", eg.engine_name.c_str());
    if (eg.engine_name == target ) {
      return eg; 
    }
  }

  return {};
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
        DEBUG_PRINT("Input text: %s\n", e->getInputText().c_str());
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
    if ( !m_default_font.loadFromFile( fonts[3] ) ){
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

  sf::Color text_color = sf::Color::White;                                 
  sf::Color background_color = {53, 98, 232, 255}; // forest green
  sf::Color hover_color = {8, 60, 212, 255}; // lighter green
  sf::Color outline_color = {8, 60, 212, 255};

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
    // put the engine data into engine_vector
    // load that engine
    syxd::InputBox* input_box = dynamic_cast<syxd::InputBox*>( m_user_interface.FindElement("name_input_box") );
    if (input_box != nullptr) {
      new_scene_name = input_box->getTextElement().getString();
    }

    if (new_scene_name == "") {
      DEBUG_PRINT("Type in something in the input box please!\n");
      return;
    }
    Engine_Block eb = findEngineByNameFromBlock(new_scene_name);
    if (eb.engine_name != "") 
    {
      DEBUG_PRINT("Scene already exists\n");
      return;
    }
    
    WINDOW_SETTINGS DEFAULT_WINDOW_SETTINGS = { 144.0f, 1600, 900, 15000, new_scene_name };
    Engine_Block temp_block;

    temp_block.engine_name = new_scene_name;
    temp_block.objects = {};
    std::string ws = std::to_string(DEFAULT_WINDOW_SETTINGS.MAX_FRAME_RATE)+","+
      std::to_string(DEFAULT_WINDOW_SETTINGS.DEFAULT_WINDOW_SIZE_X)+","+
      std::to_string(DEFAULT_WINDOW_SETTINGS.DEFAULT_WINDOW_SIZE_Y)+","+
      std::to_string(DEFAULT_WINDOW_SETTINGS.WORLD_SIZE)+","+
      DEFAULT_WINDOW_SETTINGS.WINDOW_NAME+";";

    temp_block.window_settings = ws; 


    m_engines.push_back(temp_block);
    m_show_input = !m_show_input;
    reinitialize = true;
    
  };

  load_engines = [this](){ 
    if ( bool loaded = loadAllScenes( m_scenes_path ) ){
      DEBUG_PRINT("Loaded\n");
      m_show_input = !m_show_input;
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
                              text_color, // text color
                              background_color, // background color
                              hover_color, // hover color
                              outline_color, // outline color
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
                              text_color, // text color
                              background_color, // background color
                              hover_color, // hover color
                              outline_color, // outline color
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
                              text_color, // text color
                              background_color, // background color
                              hover_color, // hover color
                              outline_color, // outline color
                              sf::Vector2f{ 150.f, 40.f }, // size
                              {0,0}, //padding
                              true, // hover
                              load_engines ); // perform this action on click
  
  syxd::Button* load_button = dynamic_cast<syxd::Button*>( m_user_interface.FindElement(load_engines_button_name) );
}

void Scene::loadEnginesToUI(){
  float engine_list_init_position = 220.0f;
  
  if ( m_engines.size() != 0 ){
    for (int i = 0; i < m_engines.size(); i++){
      float offset = 50.f;
      float y_position = engine_list_init_position+offset*(i);

      std::string button_name = m_engines.at(i).engine_name;
      std::string delete_name = "Delete";
      std::function<void()> run_scene;
      std::function<void()> delete_engine;

      sf::Color text_color = sf::Color::White;                                 
      sf::Color background_color = {160, 122, 255, 255}; // forest green
      sf::Color hover_color = {114, 60, 250, 255}; // lighter green
      sf::Color outline_color = {114, 60, 250, 255};

      run_scene = [this, button_name, i]() {
        bool loaded = this->loadScene(button_name);
        if (loaded) {
          DEBUG_PRINT("Loaded %s\n", button_name.c_str());
          runScene(); // and then run the current scene (ie the engine)
          m_engine_instance.reset();
          reinitialize = true; // reinitializeUI
        }
      };

      delete_engine = [this, button_name, delete_name]() {
        bool deleted = this->deleteEngine(button_name);
        if (deleted){
          DEBUG_PRINT("Deleted %s\n", button_name.c_str());
          m_engine_instance.reset();
          saveEnginesToFile();
          reinitialize = true; // reinitializeUI
        }
      };



      m_user_interface.InitButton(button_name, 
                                  sf::Vector2f{ 100.f, y_position },
                                  button_name, 
                                  text_color,
                                  background_color,
                                  hover_color,
                                  outline_color,
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
    
  } else DEBUG_PRINT("No engines are loaded, nothing to show\n");

}