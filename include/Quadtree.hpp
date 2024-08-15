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
  
  int threshold;
  int depth;
  bool divided = false;
  AbstractBox<float> bounds;
  std::vector<std::shared_ptr<Object>> objects;
  
  std::shared_ptr<Quadtree> top_left{};
  std::shared_ptr<Quadtree> top_right{};
  std::shared_ptr<Quadtree> bottom_left{};
  std::shared_ptr<Quadtree> bottom_right{};
  
  Quadtree( AbstractBox<float> mbox, int maxThreshold, int maxDepth ) : 
    bounds(mbox), 
    threshold(maxThreshold),
    depth(maxDepth) { }
    
  Quadtree( ) { }
  
  bool subdivideTree ( );
  
  bool insert ( std::shared_ptr<Object> obj );
  
  std::vector<std::shared_ptr<Object>> query( AbstractBox<float> range );
  
  void drawBox( std::shared_ptr<sf::RenderWindow> window );
   
};


