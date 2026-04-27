#include <Scene.hpp>

Scene::Scene( const WINDOW_SETTINGS& window_settings ) : m_window_settings{window_settings} {

  if ( bool loaded = loadAllScenes( m_scenes_path ) ){
    std::cout << "Loaded";
  }
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

bool Scene::loadScene( std::string& engine_name ) {
  if (!m_engines) return false;

  Engine_Data* target_engine = findEngineByName( engine_name );

  if (target_engine != nullptr){
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

static inline string trim(const string& s) {
  size_t a = s.find_first_not_of(" \t\r\n");
  if (a == string::npos) return "";
  size_t b = s.find_last_not_of(" \t\r\n");
  return s.substr(a, b - a + 1);
}

// Split CSV line by commas, but keep quoted fields intact
vector<string> splitCSV(const string& line) {
  vector<string> out;
  std::string cur;
  bool inQuotes = false;
  for (size_t i = 0; i < line.size(); ++i) {
    char c = line[i];
    if (c == '"' ) {
      inQuotes = !inQuotes;
      continue;
    }
    if (c == ',' && !inQuotes) {
      out.push_back(cur);
      cur.clear();
    } else {
      cur.push_back(c);
    }
  }
  out.push_back(cur);
  for (auto &s : out) s = trim(s);
  return out;
}


std::unique_ptr<Object> Object::deserializeCSV(const std::vector<std::string>& tokens) {
  // tokens expected: id,type,mass,pos_x,pos_y,dim_x,dim_y,rad,glow
  if (tokens.size() < 9) return nullptr;
  std::string type = tokens[1];
  try {
    if (type == "Rectangle") {
      float mass = tokens[2].empty() ? 0.0f : std::stof(tokens[2]);
      float px = tokens[3].empty() ? 0.0f : std::stof(tokens[3]);
      float py = tokens[4].empty() ? 0.0f : std::stof(tokens[4]);
      float dx = tokens[5].empty() ? 0.0f : std::stof(tokens[5]);
      float dy = tokens[6].empty() ? 0.0f : std::stof(tokens[6]);
      bool glow = (!tokens[8].empty() && tokens[8] != "0") ? true : false;
      return std::make_unique<syxd::Rectangle>(mass, px, py, dx, dy, glow);
    } else if (type == "Circle") {
      float mass = tokens[2].empty() ? 0.0f : std::stof(tokens[2]);
      float px = tokens[3].empty() ? 0.0f : std::stof(tokens[3]);
      float py = tokens[4].empty() ? 0.0f : std::stof(tokens[4]);
      float rad = tokens[7].empty() ? 0.0f : std::stof(tokens[7]);
      bool glow = (!tokens[8].empty() && tokens[8] != "0") ? true : false;
      return std::make_unique<syxd::Circle>(rad, mass, px, py, glow);
    }
  } catch (...) {
    return nullptr;
  }
  return nullptr;
}

void writeQuotedLine(std::ofstream& os, const string& s) {
  os << '"' << s << '"' << "\n";
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

bool startsWith(const string& s, const string& prefix) {
  return s.rfind(prefix, 0) == 0;
}

vector<Engine_Data> Scene::loadEnginesFromFile( const std::string& filename ) {
    std::ifstream is(filename);
    vector<Engine_Data> result;
    
    if (!is) {
      std::cout << "Couldn't open file\n";
      return result;
    }

    string line;

    // optional header
    if (!std::getline(is, line)) return result;
    if (!startsWith(line, "# ENGINE_FILE")) {
        // warn but continue
    }

    while (std::getline(is, line)) {
        // strip comments (must follow semicolon)
        {
            size_t semi = line.find(';');
            size_t comment = line.find("//");
            if (comment != std::string::npos && semi != std::string::npos && comment > semi)
                line = line.substr(0, comment);
        }

        line = trim(line);
        if (line.empty()) continue;

        if (line == "ENGINE") {
            Engine_Data ed;

            // engine name
            if (!std::getline(is, line)) break;
            {
                size_t semi = line.find(';');
                size_t comment = line.find("//");
                if (comment != std::string::npos && semi != std::string::npos && comment > semi)
                    line = line.substr(0, comment);
            }
            line = trim(line);

            if (!line.empty() && line.front() == '"' && line.back() == '"')
                line = line.substr(1, line.size() - 2);

            ed.window_settings.WINDOW_NAME = line;

            // find [OBJECTS]
            while (std::getline(is, line)) {
                {
                    size_t semi = line.find(';');
                    size_t comment = line.find("//");
                    if (comment != std::string::npos && semi != std::string::npos && comment > semi)
                        line = line.substr(0, comment);
                }
                line = trim(line);
                if (line.empty()) continue;

                if (line == "[OBJECTS]") {
                    // read objects
                    while (std::getline(is, line)) {
                        {
                            size_t semi = line.find(';');
                            size_t comment = line.find("//");
                            if (comment != std::string::npos && semi != std::string::npos && comment > semi)
                                line = line.substr(0, comment);
                        }
                        line = trim(line);
                        if (line.empty()) continue;

                        if (line == "[WINDOW_SETTINGS]") break;

                        if (!line.empty() && line.back() == ';')
                            line.pop_back();

                        auto tokens = splitCSV(line);
                        while (tokens.size() < 9) tokens.push_back("");

                        auto obj = Object::deserializeCSV(tokens);
                        if (obj) ed.p_objects.push_back(std::move(obj));
                    }

                    // window settings line
                    if (!std::getline(is, line)) break;

                    {
                        size_t semi = line.find(';');
                        size_t comment = line.find("//");
                        if (comment != std::string::npos && semi != std::string::npos && comment > semi)
                            line = line.substr(0, comment);
                    }

                    line = trim(line);
                    if (!line.empty() && line.back() == ';')
                        line.pop_back();

                    auto wsTokens = splitCSV(line);

                    try {
                        if (wsTokens.size() >= 5) {
                            ed.window_settings.MAX_FRAME_RATE =
                                wsTokens[0].empty() ? 0.0f : std::stof(wsTokens[0]);
                            ed.window_settings.DEFAULT_WINDOW_SIZE_X =
                                wsTokens[1].empty() ? 0 : static_cast<uint16_t>(std::stoi(wsTokens[1]));
                            ed.window_settings.DEFAULT_WINDOW_SIZE_Y =
                                wsTokens[2].empty() ? 0 : static_cast<uint16_t>(std::stoi(wsTokens[2]));
                            ed.window_settings.WORLD_SIZE =
                                wsTokens[3].empty() ? 0 : static_cast<uint16_t>(std::stoi(wsTokens[3]));

                            string wn = wsTokens[4];
                            if (!wn.empty() && wn.front() == '"' && wn.back() == '"')
                                wn = wn.substr(1, wn.size() - 2);

                            ed.window_settings.WINDOW_NAME = wn;
                        }
                    } catch (...) {
                        // ignore parse errors
                    }

                    break; // finished ENGINE block
                }
            }

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
  while ( m_is_running ) { 
    if (MAIN_WINDOW != NULL) {
      MAIN_WINDOW->clear(m_background_color );
    }
  
    float delta_time = clock.restart().asSeconds();
    EventManager( delta_time );

    m_user_interface.RenderUI( delta_time );
    
    if (MAIN_WINDOW != NULL) {
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
                              sf::Vector2f{15,15},
                              m_text_color);

  m_user_interface.InitButton("button 1", 
    sf::Vector2f{50,50}, "New Button", 
    sf::Color::White, 
    sf::Color::Red,
    sf::Color::Yellow, 
    sf::Vector2f{100.f, 40.f}, {0,0}, NULL );
  
  
}