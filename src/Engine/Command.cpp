#include <Engine.hpp>
#include <Command.hpp>

#define DEBUG 1
#if DEBUG == 1
#define DEBUG_PRINT(format, ...) printf(format, ##__VA_ARGS__)
#else
#define DEBUG_PRINT(format, ...) // Do nothing //
#endif

const std::map <std::string, Receiver::CommandType> Receiver::m_command_map = { 
      { "spawn", SPAWN }, 
      { "mode", MODE }, 
      { "exit", EXIT }, 
      { "friction", DRAG },
      { "drag", DRAG },
      { "man", MANUAL },
      { "manual", MANUAL },
      { "help", MANUAL },
      { "gravity", GRAVITY },
      { "speed", SPEED }
      
};

std::shared_ptr<Receiver> Receiver::p_instance;
std::shared_ptr<ObjectFactory> p_factory = ObjectFactory::GetInstance();

//Receiver is a singleton
std::shared_ptr<Receiver> Receiver::GetInstance() {
  if ( p_instance == nullptr ){
    p_instance = std::make_shared<Receiver>();
  }

  return p_instance;
}

// Breaks down string into words (i.e deconstructs command)
std::vector<std::string> Receiver::DeconstructCommand( const std::string& str ) {
  std::vector<std::string> words;

  size_t start = 0, end = str.find(' ');

  while (end != std::string::npos) {
    words.push_back(str.substr(start, end - start));
    start = end + 1;
    end = str.find(' ', start);
  }
  
  if (start != str.length()) {
    words.push_back(str.substr(start));
  }
  return words;
}

void Receiver::NormalizeString( std::string& string ) {
  std::transform(string.begin(), string.end(), string.begin(), [](char c) { return std::tolower(c); });
}

// converts elements in a string vector to lower case
void Receiver::NormalizeString( std::vector<std::string>& string_vector ) {
  for ( std::string& elem : string_vector ) {
    std::transform( elem.begin(), elem.end(), elem.begin(), [](char c) { return std::tolower(c); });
  }
}

// Receives commands 
bool Receiver::Receive( const std::string command, Engine* engine_instance ) {  
  std::vector<std::string> commands = DeconstructCommand( command );
  if (commands.size() < 1) {
    DEBUG_PRINT("Enter a valid command!", " ");
    return false;
  }
  NormalizeString( commands ); 
  // commands are being recieved properly
  return CallCommand( commands, engine_instance );
}

// Calls a command and executes the appropriate function
bool Receiver::CallCommand( std::vector<std::string> commands, Engine* engine_instance ) {
  CommandType command = StringToCommand( m_command_map, commands[0] );
  try
  { 
    switch ( command ){
      case SPAWN:   
        if (commands.size() < 7)
        {
          std::cout << "Enter atleast 7 arguments\n";
          break;
        }
        
        else {
 
          int num_objects;
          float centerX, centerY, radius, angleIncrement;
          
          
          if ( commands.size() < 8 ) {
            num_objects = 1;
          } else {
            num_objects = (isNumber (commands[7])) ? 
              std::stoi(commands[7]) : num_objects = 1;
          }    
          AbstractBox<float> range { Vec2( engine_instance->WINDOW->mapPixelToCoords( sf::Vector2i {0, 0} ) ) , Vec2(  engine_instance->getMainView().getSize() )*1.0f } ;
          
          NormalizeString(commands[1]);
          Object::ObjectType type = StringToCommand( Object::m_object_type_map, commands[1] );
          centerX = isNumber( commands[5] ) ? std::stof(commands[5]) : engine_instance->getMousePosf().x;
          centerY = isNumber( commands[6] ) ? std::stof(commands[6]) : engine_instance->getMousePosf().y;
          radius = isNumber( commands[3] ) ? std::stof(commands[3]) : 30.0f;
          angleIncrement = (2 * MY_PI) / num_objects;
                    
          for (int i = 0; i < num_objects; i++) {
            float posX, posY;
            if (num_objects > 1){
              float angle = i * angleIncrement;
              posX = centerX + radius * cos(angle);
              posY = centerY + radius * sin(angle);
            }
            
            else{
              posX = centerX;
              posY = centerY;
            }
            std::shared_ptr<Object> obj = (
              p_factory->createObject( type, 
                                     std::stof(commands[2]), // mass
                                     std::stof(commands[3]),  // dimension.x
                                     std::stof(commands[4]),  // dimension.y
                                     ( commands[5] == "random" || commands[5] == "rand" ) ? rand() % (int) range.getRight()+ (int)range.left : posX, // position.x
                                     ( commands[6] == "random" || commands[6] == "rand" ) ? rand() % (int) range.getBottom()+ (int)range.top : posY)); // position.y
            assert ( obj != nullptr );
            
            obj->setID(engine_instance->getAllObjects().size()+1);
            engine_instance->addObject(obj);
            if ( engine_instance->m_quad_root != nullptr && obj != nullptr ) 
            {
                engine_instance->m_quad_root->insert( obj );
            }
          }
        }
      break;
  
      case MODE:
        if (commands.size() < 2) {
          std::cout << "Enter atleast 2 arguments\n";
          break;
        }
        
        if ( commands[1] == "single" ) {
          engine_instance->setSelectMode(false);
          engine_instance->objectDefault();
        }
  
        else if ( commands[1] == "multi" ) {
          engine_instance->setSelectMode(true);
          engine_instance->objectDefault();
        }
      break;
  
      case DRAG:
        if ( commands.size() < 2 )
        {
          std::cout << "Enter atleast 2 arguments\n";
          break;
        }
        if ( commands[1] == "off" ){
          engine_instance->setDrag(0.00000000001f);
        }
  
        else if ( commands[1] == "on" ){
          engine_instance->setDrag(engine_instance->getDefaultDrag());
        }
  
        else{
          if (isNumber(commands[1])) engine_instance->setDrag(stof(commands[1]));
        }
      break;
      
      case MANUAL:
        if ( commands.size() == 1 ) {
          std::cout << "Manual Page\n";
          std::cout << "spawn circle/rectangle mass size.x size.y position.x position.y\n";
          std::cout << "friction on/off/any number value \n";   
          std::cout << "mode single/multi \n";
          std::cout << "speed number \n";
          std::cout << "exit \n"; 
          
          const WINDOW_SETTINGS help_window_settings = { 144.0f, 1000, 600, 0, "Help"};

          engine_instance->createHelpWindow( help_window_settings );
        }
        
        else if ( commands.size() >= 2 ) {
          
        }
      
      
      break;
      
      case GRAVITY:
        if ( commands.size() > 1 ) {
          NormalizeString(commands[1]);
          if ( commands[1] == "on" ) {
            engine_instance->setGravityMode(true);
          } else if ( commands[1] == "off" ) {
            engine_instance->setGravityMode(false);
          }   
        }
      break;

      case SPEED:
        if ( commands.size() > 1 ){
          if ( isNumber(commands[1]) ){
            std::cout << commands[1] << "\n";
            engine_instance->setSimulationSpeed( std::stof(commands[1]) );
          }
        }
      
      break;

      case EXIT:
        engine_instance->WINDOW->close();
      break;
    }
  } catch( const std::exception& e ) {
    std::cerr << "ERROR: " << e.what() << '\n';
    return false;
  }
  return true;
}

/* Returns true if target string is number */
bool Receiver::isNumber( const std::string& target ) {
  std::istringstream iss(target);
  double d;
  return iss >> std::noskipws >> d && iss.eof();
}