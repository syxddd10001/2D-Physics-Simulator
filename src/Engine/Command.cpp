#include "Command.hpp"


#define DEBUG 1
#if DEBUG == 1
#define DEBUG_PRINT(format, ...) printf(format, ##__VA_ARGS__)
#else
#define DEBUG_PRINT(format, ...) // Do nothing //
#endif

Receiver* Receiver::p_instance = nullptr;
ObjectFactory* factory = ObjectFactory::GetInstance();

//Receiver is a singleton
Receiver* Receiver::GetInstance(){
  if (p_instance == nullptr){
    p_instance = new Receiver();
  }

  return p_instance;
}

//Converts a String to a Command (Enum Type)
Receiver::CommandType Receiver::StringToCommand( const std::string& cmd ){
  auto it = commandMap.find(cmd);
  if (it != commandMap.end()){
    return it->second;
  } else{
    return static_cast<CommandType>(-1);        
  }

}

// Breaks down string into words (i.e deconstructs command)
std::vector<std::string> Receiver::DeconstructCommand( const std::string& str ){
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
void Receiver::NormalizeString( std::vector<std::string>& string_vector ){
  for ( std::string& elem : string_vector ){
    std::transform(elem.begin(), elem.end(), elem.begin(), [](char c) { return std::tolower(c); });
  }
}

// Receives commands 
bool Receiver::Receive( const std::string command, Engine* engine_instance ){
    
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
bool Receiver::CallCommand( std::vector<std::string> commands, Engine* engine_instance ){
  CommandType command = StringToCommand( commands[0] );
  switch ( command ){
    case SPAWN:   
      if (commands.size() < 7)
      {
          std::cout << "Enter atleast 7 arguments\n";
          break;
      }
      
      if ( commands[1] == "circle" ){
          Object* cir = factory->createObject( Object::CIRCLE, std::stof(commands[2]), std::stof(commands[3]), std::stof(commands[4]), 
                              std::stof(commands[5]), std::stof(commands[6]) );
          cir->setID(engine_instance->GetAllObjects()->size()+1);
          engine_instance->addObject(cir);
          
          
      } 
      
      else if ( commands[1] == "rectangle" ){
          Object* rec = factory->createObject( Object::RECTANGLE, std::stof(commands[2]), std::stof(commands[3]), std::stof(commands[4]), 
                              std::stof(commands[5]), std::stof(commands[6])); 
          rec->setID(engine_instance->GetAllObjects()->size()+1);
          engine_instance->addObject(rec);
      }
        
      std::cout << "Spawned Object\n";
    break;

    case MODE:
      if (commands.size() < 2) 
      {
        std::cout << "Enter atleast 2 arguments\n";
        break;
      }
      
      if ( commands[1] == "single" ){
        engine_instance->select_mode = false;
        engine_instance->objectDefault();
      }

      else if ( commands[1] == "multi" ){
        engine_instance->select_mode = true;
        engine_instance->objectDefault();
      }
    break;

    case FRICTION:
        
      if ( commands.size() < 2 ) 
      {
        std::cout << "Enter atleast 2 arguments\n";
        break;
      }
      if ( commands[1] == "off" ){
        engine_instance->friction = 0.00000000001f;
      }

      else if ( commands[1] == "on" ){
        engine_instance->friction = engine_instance->default_friction;
      }

      else{
        engine_instance->friction = stof(commands[1]);
      }
    break;
    
    case MANUAL:
      if ( commands.size() == 1 ) {
      
      }
      
      else if ( commands.size() >= 2 ){
        
      }
    
    
    break;
    
    case EXIT:
      engine_instance->WINDOW->close();
    break;
  }
  
  return true;
}
