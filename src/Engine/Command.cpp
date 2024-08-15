#include "Command.hpp"

#define DEBUG 1
#if DEBUG == 1
#define DEBUG_PRINT(format, ...) printf(format, ##__VA_ARGS__)
#else
#define DEBUG_PRINT(format, ...) // Do nothing //
#endif

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

// converts elements in a string vector to lower case
void Receiver::NormalizeString( std::vector<std::string>& string_vector ) {
  for ( std::string& elem : string_vector ){
    std::transform(elem.begin(), elem.end(), elem.begin(), [](char c) { return std::tolower(c); });
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
        
        if ( commands[1] == "circle" || commands[1] == "cir" ){
          int num_objects;
          if ( commands.size() < 8 ) num_objects = 1;
          else num_objects = std::stoi(commands[7]);
          for (int i = 0; i < num_objects; i++){
              std::shared_ptr<Circle> cir = std::dynamic_pointer_cast<Circle>(p_factory->createObject( Object::CIRCLE, std::stof(commands[2]), std::stof(commands[3]), std::stof(commands[4]), 
                                 std::stof(commands[5])+(((std::stof(commands[3])*2)) * (i+1)), std::stof(commands[6]) ));
              assert ( cir != nullptr );
              cir->setID(engine_instance->getAllObjects().size()+1);
              engine_instance->addObject(cir);
              if ( engine_instance->m_root != nullptr ) engine_instance->m_root->insert( cir );
            }
        } 
        
        else if ( commands[1] == "rectangle" || commands[1] == "rec" ){
          std::shared_ptr<Rectangle>  rec = std::dynamic_pointer_cast<Rectangle>(p_factory->createObject( Object::RECTANGLE, std::stof(commands[2]), std::stof(commands[3]), std::stof(commands[4]), 
                              std::stof(commands[5]), std::stof(commands[6]))); 
          assert ( rec != nullptr );
          rec->setID(engine_instance->getAllObjects().size()+1);
          engine_instance->addObject(rec);
          if ( engine_instance->m_root != nullptr ) engine_instance->m_root->insert( rec );
            
        }
          
        std::cout << "Spawned Object\n";
      break;
  
      case MODE:
        if (commands.size() < 2) {
          std::cout << "Enter atleast 2 arguments\n";
          break;
        }
        
        if ( commands[1] == "single" ) {
          engine_instance->m_select_mode = false;
          engine_instance->objectDefault();
        }
  
        else if ( commands[1] == "multi" ) {
          engine_instance->m_select_mode = true;
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
          engine_instance->m_drag = 0.00000000001f;
        }
  
        else if ( commands[1] == "on" ){
          engine_instance->m_drag = engine_instance->m_default_drag;
        }
  
        else{
          engine_instance->m_drag = stof(commands[1]);
        }
      break;
      
      case MANUAL:
        if ( commands.size() == 1 ) {
          std::cout << "Manual Page\n";
          std::cout << "spawn circle/rectangle mass size.x size.y position.x position.y\n";
          std::cout << "friction on/off/any number value \n";   
          std::cout << "mode single/multi \n";
          std::cout << "exit \n"; 
        }
        
        else if ( commands.size() >= 2 ) {
          
        }
      
      
      break;
      
      case EXIT:
        engine_instance->WINDOW->close();
      break;
    }
  } catch(const std::exception& e) {
    std::cerr << e.what() << '\n';
    return false;
  }
  
  return true;
}

