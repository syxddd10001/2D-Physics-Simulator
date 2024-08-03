#include <string>
#include "Object.hpp"
#include "Circle.hpp"
#include "Rectangle.hpp"

// Where objects get created depending on the input
class ObjectFactory {
private:
  static ObjectFactory* p_instance_factory;
  ObjectFactory() {};

public:
  ObjectFactory( ObjectFactory& other ) = delete;

  void operator=( const ObjectFactory& ) = delete;

  static ObjectFactory* GetInstance();
    
  Object* createObject( Object::ObjectType object_type, const float mass, float dim_x, float dim_y, float pos_x, float pos_y  );
};