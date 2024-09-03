#pragma once
#include <vector>
#include <memory>
#include <cassert>
#include <iostream>
#include <queue>
#include <thread>
#include <mutex>

#include "AbstractBox.hpp"
#include "Object.hpp"
#include "Circle.hpp"
#include "Rectangle.hpp"
#include <SFML/Graphics.hpp>

using namespace syxd;

class Quadtree {
private:
  const float THETA = 2.0f;
  
  int m_threshold;
  int m_depth;
  bool m_divided = false;
  
  Vec2 m_node_center_of_mass;
  float m_node_mass_total = 0.0f;
  float m_node_net_force = 0.f;
  
  
  AbstractBox<float> m_bounds;
  std::vector<std::shared_ptr<Object>> m_objects;
  
  std::shared_ptr<Quadtree> m_top_left{};
  std::shared_ptr<Quadtree> m_top_right{};
  std::shared_ptr<Quadtree> m_bottom_left{};
  std::shared_ptr<Quadtree> m_bottom_right{};

public:

  Quadtree( AbstractBox<float> mbox, int maxThreshold, int maxDepth ) : 
    m_bounds(mbox), 
    m_threshold(maxThreshold),
    m_depth(maxDepth) { }
    
  Quadtree( ) { }
  
  bool subdivideTree ( );
  
  bool insert( std::shared_ptr<Object> obj );
  
  std::vector<std::shared_ptr<Object>> query( AbstractBox<float> range );
  
  void drawBox( std::shared_ptr<sf::RenderWindow> window );

  Vec2 calculateForce( std::shared_ptr<Object> obj );
   
};


