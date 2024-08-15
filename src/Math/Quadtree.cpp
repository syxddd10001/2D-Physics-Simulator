#include <Quadtree.hpp>

bool Quadtree::subdivideTree ( ) {
  AbstractBox<float> topleft { bounds.getTopLeft() , Vec2 { bounds.getSize().x/2, bounds.getSize().y/2 } } ;
  top_left = std::make_shared<Quadtree>( topleft, threshold, depth );

  AbstractBox<float> topright { Vec2 { bounds.getCenter().x, bounds.getTopLeft().y } , Vec2 { bounds.getSize().x/2, bounds.getSize().y/2 }} ;
  top_right = std::make_shared<Quadtree>( topright, threshold, depth );

  AbstractBox<float> bottomleft { Vec2 { bounds.getTopLeft().x, bounds.getCenter().y } , Vec2 { bounds.getSize().x/2, bounds.getSize().y/2 } } ;
  bottom_left = std::make_shared<Quadtree>( bottomleft, threshold, depth );
  
  AbstractBox<float> bottomright { Vec2 { bounds.getCenter().x, bounds.getCenter().y }, Vec2 { bounds.getSize().x/2, bounds.getSize().y/2 } } ;
  bottom_right = std::make_shared<Quadtree>( bottomright, threshold, depth );
  
  divided = true;
  return divided;
}
  
bool Quadtree::insert ( std::shared_ptr<Object> obj ) {
  if ( !( this->bounds.contains( obj->getPosition(), obj->getSize() ) ) ){
    return false;
  }
  
  if ( objects.size() < threshold && !top_left ){
    objects.push_back( obj );
    return true;
    
  } else {
    if ( !top_left ) {
      subdivideTree( );
    }
    
    if (top_left->insert( obj )) return true;
    if (top_right->insert( obj )) return true;
    if (bottom_left->insert( obj )) return true;
    if (bottom_right->insert( obj )) return true;
    
  }
  return false;
}

std::vector<std::shared_ptr<Object>> Quadtree::query( AbstractBox<float> range ) {
  std::vector<std::shared_ptr<Object>> objects_in_range;
  
  if ( !bounds.intersects(range) ) {
    return objects_in_range;
  }
  
  for ( size_t p = 0; p<objects.size(); p++ ) {
    if ( range.contains( objects[p]->getCenter(), objects[p]->getSize() ) ) {
      objects_in_range.push_back( objects[p] );
    }
  }
  
  if ( top_left == nullptr ) {
    return objects_in_range;
  }
  
  std::vector<std::shared_ptr<Object>> temp1 = top_left->query( range );
  objects_in_range.insert(objects_in_range.end(), temp1.begin(), temp1.end() );
  std::vector<std::shared_ptr<Object>> temp2 = top_right->query( range );
  objects_in_range.insert(objects_in_range.end(), temp2.begin(), temp2.end() );
  std::vector<std::shared_ptr<Object>> temp3 = bottom_left->query( range );
  objects_in_range.insert(objects_in_range.end(), temp3.begin(), temp3.end() );
  std::vector<std::shared_ptr<Object>> temp4 = bottom_right->query( range );
  objects_in_range.insert(objects_in_range.end(), temp4.begin(), temp4.end() );
  
  return objects_in_range;
}

void Quadtree::drawBox( std::shared_ptr<sf::RenderWindow> window ) {
  window->draw( *(bounds.shape) );
  
  if (divided) {
    if (top_left != nullptr ) top_left->drawBox( window );
    if (top_right != nullptr ) top_right->drawBox( window );
    if (bottom_left != nullptr ) bottom_left->drawBox( window );
    if (bottom_right != nullptr ) bottom_right->drawBox( window );
  }
}