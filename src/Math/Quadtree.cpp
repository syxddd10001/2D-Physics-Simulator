#include <Quadtree.hpp>

/* 
  Create 4 children nodes
*/
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
  // if the current node has space and its not subdivided
  if ( m_objects.size() < m_threshold && !m_divided ){
    m_objects.push_back( obj );
    Vec2 old_com = m_node_center_of_mass;
    float old_mass = m_node_mass_total;
    m_node_mass_total = obj->getMass()+old_mass;
    m_node_center_of_mass = (old_com * old_mass + obj->getPosition() * obj->getMass()) / m_node_mass_total;
    return true;
  }
  // node has no room
  if ( !m_divided ) { // node is external and not divided
    Vec2 com { 0.f, 0.f }; // calculate total mass and center of mass
    for ( const auto& o : m_objects ) {
      float m = o->getMass();
      m_node_mass_total += m;
      com += o->getPosition() * m;
    }
    m_node_center_of_mass = com/m_node_mass_total;
    subdivideTree( ); // and then subdivide tree
  }
    
  // calculate new total mass and center of mass
  // node_com = (old_com * old node mass + obj mass * obj pos) / new total mass 
  Vec2 old_com = m_node_center_of_mass;
  float old_mass = m_node_mass_total;
  m_node_mass_total = obj->getMass()+old_mass;
  m_node_center_of_mass = (old_com * old_mass + obj->getPosition() * obj->getMass()) / m_node_mass_total;  
    
  // when subdivided, insert the object to one of the 4 children nodes
  if ( m_top_left->insert( obj ) ) return true;
  if ( m_top_right->insert( obj ) ) return true;
  if ( m_bottom_left->insert( obj ) ) return true;
  if ( m_bottom_right->insert( obj ) ) return true;
  
  return false;
}
/*
  Query and return all objects contained in range
*/
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
  
  if ( m_top_left == nullptr ) { // if current node is leaf
    return objects_in_range;
  }
  
  std::vector<std::shared_ptr<Object>> temp1 = m_top_left->query( range );
  objects_in_range.insert( objects_in_range.end(), temp1.begin(), temp1.end() );
  std::vector<std::shared_ptr<Object>> temp2 = m_top_right->query( range );
  objects_in_range.insert( objects_in_range.end(), temp2.begin(), temp2.end() );
  std::vector<std::shared_ptr<Object>> temp3 = m_bottom_left->query( range );
  objects_in_range.insert( objects_in_range.end(), temp3.begin(), temp3.end() );
  std::vector<std::shared_ptr<Object>> temp4 = m_bottom_right->query( range );
  objects_in_range.insert( objects_in_range.end(), temp4.begin(), temp4.end() );

  
  return objects_in_range;
}

/*
  Draw quadtree recursively into target window 
*/
void Quadtree::drawBox( std::shared_ptr<sf::RenderWindow> window ) {
  window->draw( *(m_bounds.shape) );
  
  if (m_divided) {
    if (m_top_left != nullptr ) m_top_left->drawBox( window );
    if (m_top_right != nullptr ) m_top_right->drawBox( window );
    if (m_bottom_left != nullptr ) m_bottom_left->drawBox( window );
    if (m_bottom_right != nullptr ) m_bottom_right->drawBox( window );
  }
}

/*
  Using Barnes Hutt algorithm to calculate force on obj
*/
Vec2 Quadtree::calculateForce( std::shared_ptr<Object> obj ){ 
  /* 
    Calculating force on obj
  
    leaf node = external node = not divided => calculate force, add force to obj's netforce
    
    if internal = divided => calculate ratio s / d where s = the width of region and d is the distance from COM to obj's pos
  
    if s/d < theta (arbitrary - pick any > zero), treat this interal node as a single body, and calculate force on obj
    add this force to obj's netforce
    
    if current node is internal and ratio of s/d >= theta, recurse through the children
  */
  // !! refactor/optimize later !!
  Vec2 net_force { 0.f, 0.f };
  
  for ( const auto& other : m_objects ) {
    if ( other == obj ) continue;  
    Vec2 dir { other->getPosition() - obj->getPosition() };
    float dist = calculateDistance( other->getPosition(), obj->getPosition() );
    if ( dist < EPSILON ) dist = EPSILON;
    dir = dir.normalize();
    Vec2 force_exerted_by_node = (dir * MY_G_CONSTANT * obj->getMass() * other->getMass())/(dist*dist);
    net_force += force_exerted_by_node;
  }
  
  if ( m_divided ) {
    if ( m_bounds.getSize().x / calculateDistance( m_node_center_of_mass, obj->getPosition()) < THETA ) {
      // force exerted = G_constant * mass of obj * total mass of node / (distance from node_COM to obj pos)^2 * direction
      Vec2 dir { m_node_center_of_mass - obj->getPosition() };
      dir = dir.normalize();
      float dist = calculateDistance( m_node_center_of_mass, obj->getPosition() );
      if ( dist < EPSILON ) dist = EPSILON;
      Vec2 force_exerted_by_node = (dir * MY_G_CONSTANT * obj->getMass() * m_node_mass_total) / (dist*dist);
      net_force += force_exerted_by_node;   
    }
    
    else {
      net_force += m_top_left->calculateForce( obj );
      net_force += m_top_right->calculateForce( obj );
      net_force += m_bottom_left->calculateForce( obj );
      net_force += m_bottom_right->calculateForce( obj ); 
    }
  }
  return net_force;
}