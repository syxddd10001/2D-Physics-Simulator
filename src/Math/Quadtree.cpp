#include <Quadtree.hpp>

bool Quadtree::subdivideTree ( ) {
  AbstractBox<float> topleft { m_bounds.getTopLeft() , Vec2 { m_bounds.getSize().x/2, m_bounds.getSize().y/2 } } ;
  m_top_left = std::make_shared<Quadtree>( topleft, m_threshold, m_depth );

  AbstractBox<float> topright { Vec2 { m_bounds.getCenter().x, m_bounds.getTopLeft().y } , Vec2 { m_bounds.getSize().x/2, m_bounds.getSize().y/2 }} ;
  m_top_right = std::make_shared<Quadtree>( topright, m_threshold, m_depth );

  AbstractBox<float> bottomleft { Vec2 { m_bounds.getTopLeft().x, m_bounds.getCenter().y } , Vec2 { m_bounds.getSize().x/2, m_bounds.getSize().y/2 } } ;
  m_bottom_left = std::make_shared<Quadtree>( bottomleft, m_threshold, m_depth );
  
  AbstractBox<float> bottomright { Vec2 { m_bounds.getCenter().x, m_bounds.getCenter().y }, Vec2 { m_bounds.getSize().x/2, m_bounds.getSize().y/2 } } ;
  m_bottom_right = std::make_shared<Quadtree>( bottomright, m_threshold, m_depth );
  
  m_divided = true;
  return m_divided;
}
  
bool Quadtree::insert ( std::shared_ptr<Object> obj ) {
  if ( !( this->m_bounds.contains( obj->getPosition(), obj->getSize() ) ) ) {
    return false;
  }
  
  if ( m_objects.size() < m_threshold && !m_top_left ){
    m_objects.push_back( obj );
    return true;
    
  } else {
    if ( !m_top_left ) {
      subdivideTree( );
    }
    
    if ( m_top_left->insert( obj ) ) return true;
    if ( m_top_right->insert( obj ) ) return true;
    if ( m_bottom_left->insert( obj ) ) return true;
    if ( m_bottom_right->insert( obj ) ) return true;    
  }
  
  return false;
}

std::vector<std::shared_ptr<Object>> Quadtree::query( AbstractBox<float> range ) {
  std::vector<std::shared_ptr<Object>> objects_in_range;
  
  if ( !m_bounds.intersects(range) ) {
    return objects_in_range;
  }
  
  for ( size_t p = 0; p<m_objects.size(); p++ ) {
    if ( range.contains( m_objects[p]->getCenter(), m_objects[p]->getSize() ) ) {
      objects_in_range.push_back( m_objects[p] );
    }
  }
  
  if ( m_top_left == nullptr ) {
    return objects_in_range;
  }
  
  std::vector<std::shared_ptr<Object>> temp1 = m_top_left->query( range );
  objects_in_range.insert(objects_in_range.end(), temp1.begin(), temp1.end() );
  std::vector<std::shared_ptr<Object>> temp2 = m_top_right->query( range );
  objects_in_range.insert(objects_in_range.end(), temp2.begin(), temp2.end() );
  std::vector<std::shared_ptr<Object>> temp3 = m_bottom_left->query( range );
  objects_in_range.insert(objects_in_range.end(), temp3.begin(), temp3.end() );
  std::vector<std::shared_ptr<Object>> temp4 = m_bottom_right->query( range );
  objects_in_range.insert(objects_in_range.end(), temp4.begin(), temp4.end() );
  
  return objects_in_range;
}

void Quadtree::drawBox( std::shared_ptr<sf::RenderWindow> window ) {
  window->draw( *(m_bounds.shape) );
  
  if (m_divided) {
    if (m_top_left != nullptr ) m_top_left->drawBox( window );
    if (m_top_right != nullptr ) m_top_right->drawBox( window );
    if (m_bottom_left != nullptr ) m_bottom_left->drawBox( window );
    if (m_bottom_right != nullptr ) m_bottom_right->drawBox( window );
  }
}