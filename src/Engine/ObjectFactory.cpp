#include <string>
#include "ObjectFactory.hpp"

ObjectFactory* ObjectFactory::p_instance_factory = nullptr;

// ObjectFactory is a singleton
ObjectFactory* ObjectFactory::GetInstance(){
    
  if (p_instance_factory == nullptr){
    p_instance_factory = new ObjectFactory();
  }

  return p_instance_factory; 
}

// Creates an object
std::shared_ptr<Object> ObjectFactory::createObject( Object::ObjectType object_type, 
                                    const float mass, 
                                    float dim_x, float dim_y, 
                                    float pos_x, float pos_y ) {
  
  switch ( object_type )
  {
    case Object::CIRCLE:
      return std::make_shared<Circle>( dim_x, mass, pos_x, pos_y );
    break;
  
    case Object::RECTANGLE:
      return std::make_shared<Rectangle>( mass, pos_x, pos_y, dim_x, dim_y );
    break;

  }
}