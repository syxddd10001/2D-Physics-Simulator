#pragma once
#include <vector>
#include <memory>
#include <cassert>
#include <iostream>
#include <queue>
#include "AbstractBox.hpp"
#include "Object.hpp"
#include "Circle.hpp"
#include "Rectangle.hpp"
#include <SFML/Graphics.hpp>

class Quadtree {
public:
  
  int m_threshold;
  int m_depth;
  bool m_divided = false;
  AbstractBox<float> m_bounds;
  std::vector<std::shared_ptr<Object>> m_objects;
  
  std::shared_ptr<Quadtree> m_top_left{};
  std::shared_ptr<Quadtree> m_top_right{};
  std::shared_ptr<Quadtree> m_bottom_left{};
  std::shared_ptr<Quadtree> m_bottom_right{};
  
  Quadtree( AbstractBox<float> mbox, int maxThreshold, int maxDepth ) : 
    m_bounds(mbox), 
    m_threshold(maxThreshold),
    m_divided(maxDepth) { }
    
  Quadtree( ) { }
  
  bool subdivideTree ( );
  
  bool insert ( std::shared_ptr<Object> obj );
  
  std::vector<std::shared_ptr<Object>> query( AbstractBox<float> range );
  
  void drawBox( std::shared_ptr<sf::RenderWindow> window );
   
};


