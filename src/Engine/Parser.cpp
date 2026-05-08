#pragma once
#include <Parser.hpp>

void writeQuotedLine( std::ofstream& os, const string& s ) {
  os << '"' << s << '"' << "\n";
}

bool startsWith( const string& s, const string& prefix ) {
  return s.rfind(prefix, 0) == 0;
}

std::string trim( const string& s ) {
  size_t a = s.find_first_not_of(" \t\r\n");
  if (a == string::npos) return "";
  size_t b = s.find_last_not_of(" \t\r\n");
  return s.substr(a, b - a + 1);
}

std::string trimQuotes(const std::string& s) {
  std::string out = s;

  if (out.size() >= 2 && out.front() == '"' && out.back() == '"')
      out = out.substr(1, out.size() - 2);

  return out;
}


vector<string> splitCSV( const string& line ) {
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

std::string cleanLine( std::string line ) {
  // strip comments only if they come after a semicolon
  size_t semi    = line.find(';');
  size_t comment = line.find("//");
  if (comment != std::string::npos &&
      semi    != std::string::npos &&
      comment > semi)
  {
      line = line.substr(0, comment);
  }

  line = trim(line);

  // remove trailing semicolon
  if (!line.empty() && line.back() == ';')
      line.pop_back();

  return line;
}

bool readCleanLine( std::istream& is, std::string& out ) {
  std::string raw;
  if (!std::getline(is, raw))
      return false;
  out = cleanLine(raw);
  return true;
}

void parseEngineName( std::istream& is, Engine_Data& ed ) {
  std::string line;
  if (!readCleanLine(is, line))
      return;

  // strip quotes if present
  if (!line.empty() && line.front() == '"' && line.back() == '"' && line.size() >= 2)
      line = line.substr(1, line.size() - 2);

  ed.window_settings.WINDOW_NAME = line;
}

void parseObjects( std::istream& is, Engine_Data& ed ) {
  std::string line;

  // find [OBJECTS]
  while ( readCleanLine(is, line) ) {
      if (line.empty()) continue;
      if (line == "[OBJECTS]") break;
  }

  // read objects until [WINDOW_SETTINGS] or EOF
  while ( readCleanLine(is, line) ) {
      if (line.empty()) continue;
      if (line == "[WINDOW_SETTINGS]") {
          // push back marker into stream logic: we just return and let caller
          // handle window settings starting from this line
          // but easier: caller will read next line as settings, so we stop here
          // and let caller know we hit the marker.
          // We'll handle that by having caller call parseWindowSettings next.
          // So just break.
          break;
      }

      auto tokens = splitCSV(line);
      while (tokens.size() < 11) tokens.push_back("");

      if (tokens[1] == "Rectangle"){
        
      }
      else if (tokens[1] == "Circle"){

      }

      auto obj = Object::deserializeCSV(tokens);
      if (obj) ed.p_objects.push_back(std::move(obj));
  }

  // At this point, the last line read was either "[WINDOW_SETTINGS]" or EOF.
  // Caller will read the actual window settings line next.
}

void parseWindowSettings( std::istream& is, Engine_Data& ed ) {
  std::string line;
  if (!readCleanLine(is, line))
      return;

  if (line.empty())
      return;

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

          std::string wn = wsTokens[4];
          if (!wn.empty() && wn.front() == '"' && wn.back() == '"' && wn.size() >= 2)
              wn = wn.substr(1, wn.size() - 2);

          ed.window_settings.WINDOW_NAME = wn;
      }
  } catch (...) {
      // ignore parse errors
  }
}
/*
Desiralizes from CSV to Object
PARAM: tokens expected -- id,type,mass,pos_x,pos_y,dim_x,dim_y,rad,glow,vel_x,vel_y,color
*/
std::unique_ptr<Object> Object::deserializeCSV( const std::vector<std::string>& tokens ) {
  if (tokens.size() < 11) return nullptr;
  std::string type = tokens[1];
  try {
    if ( type == "Rectangle" ) {
      float mass = tokens[2].empty() ? 0.0f : std::stof( tokens[2] );
      float px = tokens[3].empty() ? 0.0f : std::stof( tokens[3] );
      float py = tokens[4].empty() ? 0.0f : std::stof( tokens[4] );
      float dx = tokens[5].empty() ? 0.0f : std::stof( tokens[5] );
      float dy = tokens[6].empty() ? 0.0f : std::stof( tokens[6] );
      bool glow = ( !tokens[8].empty() && tokens[8] != "0" ) ? true : false;
      float vx = ( tokens.size() > 9 && !tokens[9].empty() ) ? std::stof( tokens[9] ) : 0.0f;
      float vy = ( tokens.size() > 10 && !tokens[10].empty() ) ? std::stof( tokens[10] ) : 0.0f;
      long long int int_color = ( tokens.size() > 11 && !tokens[11].empty() ) ? std::stoll( tokens[11] ) : -1;
      sf::Color* color = new sf::Color( static_cast<sf::Uint32>(int_color) );
      std::unique_ptr<syxd::Rectangle> r = std::make_unique<syxd::Rectangle>( mass, 
                                                                              px, 
                                                                              py, 
                                                                              dx, 
                                                                              dy, 
                                                                              glow, 
                                                                              color );
      r->setVelocity( {vx,vy} );
      return r;

    } else if ( type == "Circle" ) {
      float mass = tokens[2].empty() ? 0.0f : std::stof( tokens[2] );
      float px = tokens[3].empty() ? 0.0f : std::stof( tokens[3] );
      float py = tokens[4].empty() ? 0.0f : std::stof( tokens[4] );
      float rad = tokens[7].empty() ? 0.0f : std::stof( tokens[7] );
      bool glow = (!tokens[8].empty() && tokens[8] != "0") ? true : false;
      float vx = (tokens.size() > 9 && !tokens[9].empty()) ? std::stof( tokens[9] ) : 0.0f;
      float vy = (tokens.size() > 10 && !tokens[10].empty()) ? std::stof( tokens[10] ) : 0.0f;

      long long int int_color = ( tokens.size() > 11 && !tokens[11].empty() ) ? std::stoll( tokens[11] ) : -1;

      sf::Color* color = new sf::Color( static_cast<sf::Uint32>(int_color) );

      std::unique_ptr<syxd::Circle> c = std::make_unique<syxd::Circle>( rad, 
                                                                       mass, 
                                                                       px, 
                                                                       py, 
                                                                       glow, 
                                                                       color );
      c->setVelocity({vx,vy});
      return c;
    }
  } catch (...) {
    return nullptr;
  }
  return nullptr;
}

std::string ensure_trailing_semicolon( std::string s ) {
  // trim right whitespace
  while (!s.empty() && (s.back() == ' ' || s.back() == '\t' || s.back() == '\r' || s.back() == '\n'))
      s.pop_back();
  if (s.empty() || s.back() == ';') return s;
  return s + ";";
}

std::vector<Engine_Block> parseEngines(std::istream& is) {
    std::vector<Engine_Block> engines;
    std::string line;

    Engine_Block current;
    bool inEngine = false;
    bool inObjects = false;
    bool inWindow = false;

    while (std::getline(is, line)) {
        // Clean whitespace
        if (line.size() && line.back() == '\r')
            line.pop_back();

        if (line == "[ENGINE]") {
            // If we were already reading an engine, push it
            if (inEngine) {
                engines.push_back(current);
                current = Engine_Block{};
            }
            inEngine = true;
            inObjects = false;
            inWindow = false;
            continue;
        }

        if (line == "[OBJECTS]") {
            inObjects = true;
            inWindow = false;
            continue;
        }

        if (line == "[WINDOW_SETTINGS]") {
            inWindow = true;
            inObjects = false;
            continue;
        }

        // Now handle content
        if (inEngine && !inObjects && !inWindow) {
            if (!line.empty())
                current.engine_name = trimQuotes(line);
        }
        else if (inObjects) {
            if (!line.empty())
                current.objects.push_back(line);
        }
        else if (inWindow) {
            if (!line.empty())
                current.window_settings = line;
        }
    }

    // Push last engine if file didn't end with a new [ENGINE]
    if (inEngine)
        engines.push_back(current);

    return engines;
}


void parseEngineName( const std::string& rawName, Engine_Data& ed )
{
    if (rawName.empty())
        return;

    std::string name =  cleanLine(trimQuotes(rawName));

    ed.window_settings.WINDOW_NAME = name;
}


void parseObjects(const std::vector<std::string>& lines, Engine_Data& ed)
{
    for (const auto& line : lines) {
        if (line.empty())
            continue;

        auto tokens = splitCSV(line);

        // Ensure minimum field count
        if (tokens.size() < 11)
            tokens.resize(11);

        auto obj = Object::deserializeCSV(tokens);
        if (obj)
            ed.p_objects.push_back(std::move(obj));
    }
}

void parseWindowSettings(const std::string& line, WINDOW_SETTINGS& ws)
{
    if (line.empty())
        return;

    auto t = splitCSV(line);
    if (t.size() < 5)
        t.resize(5);

    try {
        ws.MAX_FRAME_RATE =
            t[0].empty() ? 0.f : std::stof(t[0]);

        ws.DEFAULT_WINDOW_SIZE_X =
            t[1].empty() ? 0 : static_cast<uint16_t>(std::stoi(t[1]));

        ws.DEFAULT_WINDOW_SIZE_Y =
            t[2].empty() ? 0 : static_cast<uint16_t>(std::stoi(t[2]));

        ws.WORLD_SIZE =
            t[3].empty() ? 0 : static_cast<uint16_t>(std::stoi(t[3]));

        std::string wn = t[4];
        if (wn.size() >= 2 && wn.front() == '"' && wn.back() == '"')
            wn = wn.substr(1, wn.size() - 2);

        ws.WINDOW_NAME = cleanLine(wn);
    }
    catch (...) {
        // ignore parse errors
    }
}

Engine_Data parseEngineBlock( Engine_Block block ){
  Engine_Data ed;
  
  // Parse engine name
  parseEngineName(block.engine_name, ed );

  // Parse objects
  parseObjects(block.objects, ed);

  // Parse window settings
  parseWindowSettings(block.window_settings, ed.window_settings);

  DEBUG_PRINT("WINDOW NAME: %s\n", ed.window_settings.WINDOW_NAME.c_str());

  return ed;
}

std::vector<Engine_Data> parseAllEngineBlocks( std::vector<Engine_Block> blocks ) {
  std::vector<Engine_Data> result;
  for (const auto& block : blocks ) { 
    Engine_Data ed = parseEngineBlock( block );
    result.push_back(ed);
  }

  return result;
}

std::vector<Engine_Block> engineDataToBlockBulk( std::vector<Engine_Data> engine_data ){
  std::vector<Engine_Block> blocks;
  
  for (auto ed : engine_data ) { 
    Engine_Block b = engineDataToBlock( ed );
    blocks.push_back(b);
  }

  return blocks;
}

Engine_Block engineDataToBlock( Engine_Data engine_data ){
  Engine_Block b;
  std::vector<std::shared_ptr<Object>> objects = engine_data.p_objects; 
  std::vector<std::string> serialized_objects;
  std::string window_settings;

  b.engine_name = engine_data.window_settings.WINDOW_NAME;
  window_settings = std::to_string(engine_data.window_settings.MAX_FRAME_RATE)+","+
                    std::to_string(engine_data.window_settings.DEFAULT_WINDOW_SIZE_X)+","+
                    std::to_string(engine_data.window_settings.DEFAULT_WINDOW_SIZE_Y)+","+
                    std::to_string(engine_data.window_settings.WORLD_SIZE)+","+
                    engine_data.window_settings.WINDOW_NAME+";";

  for ( auto o : objects ){
    serialized_objects.push_back(o->serializeCSV( o->getID() ));
  }

  b.objects = serialized_objects;
  b.window_settings = window_settings;

  return b;
}