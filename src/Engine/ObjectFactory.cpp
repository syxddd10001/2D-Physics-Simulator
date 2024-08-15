#include <string>
#include "ObjectFactory.hpp"

std::shared_ptr<ObjectFactory> ObjectFactory::p_instance_factory = nullptr;

// ObjectFactory is a singleton
std::shared_ptr<ObjectFactory> ObjectFactory::GetInstance() {
    
  if (p_instance_factory == nullptr) {
    p_instance_factory = std::make_shared<ObjectFactory>();
  }

  return p_instance_factory; 
}

// Creates an object
std::shared_ptr<Object> ObjectFactory::createObject( const Object::ObjectType object_type, 
                                    const float mass, 
                                    const float dim_x, const float dim_y, 
                                    const float pos_x,  const float pos_y ) {
  
  switch ( object_type ) {
    case Object::CIRCLE:
      return std::make_shared<Circle>( dim_x, mass, pos_x, pos_y );
    break;
  
    case Object::RECTANGLE:
      return std::make_shared<Rectangle>( mass, pos_x, pos_y, dim_x, dim_y );
    break;
    
    default:
      return nullptr; // !! dangerous, make sure to take care of this !!
    break;
  }
}