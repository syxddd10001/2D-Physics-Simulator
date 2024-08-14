#include <string>
#include <memory>
#include "Object.hpp"
#include "Circle.hpp"
#include "Rectangle.hpp"

// Where objects get created depending on the input
class ObjectFactory {
private:
  static std::shared_ptr<ObjectFactory> p_instance_factory;

public:
  ObjectFactory() {};
  
  ObjectFactory( ObjectFactory& other ) = delete;
  ~ObjectFactory() {};

  void operator=( const ObjectFactory& ) = delete;

  static std::shared_ptr<ObjectFactory> GetInstance();
    
  std::shared_ptr<Object> createObject( Object::ObjectType object_type, const float mass, 
                                      const float dim_x, const float dim_y, 
                                      const float pos_x, const float pos_y );
};