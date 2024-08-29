#pragma once
#include <string>
#include <vector>
#include <algorithm>
#include <cctype>
#include <iostream>
#include <map>
#include <memory>
#include <cstring>
#include <cstdlib>
#include <ObjectFactory.hpp>
#include <Object.hpp>
#include <Engine.hpp>

class Receiver{
private:
  static std::shared_ptr<Receiver> p_instance;
  enum CommandType { SPAWN, MODE, EXIT, DRAG, MANUAL, GRAVITY };
  static const std::map <std::string, CommandType> m_command_map;
  bool CallCommand( std::vector<std::string> commands, Engine* engine_instance); //Calls a command and executes the appropriate function
  std::vector<std::string> DeconstructCommand( const std::string& str ); // Breaks down string into words (i.e deconstructs command)
  void NormalizeString( std::vector<std::string>& string_vector ); // converts elements in a string vector to lower case i.e normalizes it
  void NormalizeString( std::string& string );
  bool isNumber( const std::string& target );

public:
  Receiver() {};

  Receiver( Receiver& other ) = delete;
  ~Receiver() {};

  void operator=( const Receiver& ) = delete;

  static std::shared_ptr<Receiver> GetInstance(); 

  bool Receive( const std::string command, Engine* engine_instance ); // Receives commands 
  
  template <typename T>
  T StringToCommand( const std::map<std::string, T>& enum_map, const std::string& cmd ) { //Converts a String to a Command (Enum Type)
    auto it = enum_map.find(cmd);
    if (it != enum_map.end()){
      return it->second;
    } else{
      return static_cast<T>(-1);  
    }
  };
  
  

}; 