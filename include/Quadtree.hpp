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
  std::vector<Object*> objects;
  
  Quadtree* topLeft{};
  Quadtree* topRight{};
  Quadtree* bottomLeft{};
  Quadtree* bottomRight{};
  
  Quadtree( AbstractBox<float> mbox, int maxThreshold, int maxDepth ) : 
    bounds(mbox), 
    threshold(maxThreshold),
    depth(maxDepth) { }
    

  Quadtree( ) {
  
  }
  
  bool subdivideTree ( ) {
    AbstractBox<float> topleft { bounds.getTopLeft() , Vec2 { bounds.getSize().x/2, bounds.getSize().y/2 } } ;
    topLeft = new Quadtree( topleft, threshold, depth );
  
    AbstractBox<float> topright { Vec2 { bounds.getCenter().x, bounds.getTopLeft().y } , Vec2 { bounds.getSize().x/2, bounds.getSize().y/2 }} ;
    topRight = new Quadtree( topright, threshold, depth );
  
    AbstractBox<float> bottomleft { Vec2 { bounds.getTopLeft().x, bounds.getCenter().y } , Vec2 { bounds.getSize().x/2, bounds.getSize().y/2 } } ;
    bottomLeft = new Quadtree( bottomleft, threshold, depth );
    
    AbstractBox<float> bottomright { Vec2 { bounds.getCenter().x, bounds.getCenter().y }, Vec2 { bounds.getSize().x/2, bounds.getSize().y/2 } } ;
    bottomRight = new Quadtree( bottomright, threshold, depth );
    
    divided = true;
    return divided;
  }
  
  
  bool insert ( Object* obj ){
    if ( ! (this->bounds.contains( obj->getPosition(), obj->getSize() )) ){
      return false;
    }
    
    if ( objects.size() < threshold && topLeft == nullptr ){
      objects.push_back( obj );
      return true;
      
    } else {
      if ( !divided ) {
        subdivideTree( );
      }
      
      if (this->topLeft->insert( obj )) return true;
      else if (this->topRight->insert( obj )) return true;
      else if (this->bottomLeft->insert( obj )) return true;
      else if (this->bottomRight->insert( obj )) return true;
      
    }
    
    return false;
  }
  
  std::vector<Object*> query( AbstractBox<float> range ) {
    std::vector<Object*> objects_in_range;
    
    if ( !bounds.intersects(range) ){
      return objects_in_range;
    }
    
    for ( int p = 0; p<objects.size(); p++ ){
      if ( range.contains( objects[p]->getCenter(), objects[p]->getSize() ) ) {
        objects_in_range.push_back( objects[p] );
      }
    }
    
    if ( topLeft == nullptr ) {
      return objects_in_range;
    }
    
    std::vector<Object*> temp1 = topLeft->query( range );
    objects_in_range.insert(objects_in_range.end(), temp1.begin(), temp1.end() );
    std::vector<Object*> temp2 = topRight->query( range );
    objects_in_range.insert(objects_in_range.end(), temp2.begin(), temp2.end() );
    std::vector<Object*> temp3 = bottomLeft->query( range );
    objects_in_range.insert(objects_in_range.end(), temp3.begin(), temp3.end() );
    std::vector<Object*> temp4 = bottomRight->query( range );
    objects_in_range.insert(objects_in_range.end(), temp4.begin(), temp4.end() );
    
    return objects_in_range;
  }
  
  void drawBox( std::shared_ptr<sf::RenderWindow> window ) {
    window->draw( *(bounds.shape) );
    
    if (divided){
      if (topLeft != nullptr ) topLeft->drawBox( window );
      if (topRight != nullptr ) topRight->drawBox( window );
      if (bottomLeft != nullptr ) bottomLeft->drawBox( window );
      if (bottomRight != nullptr ) bottomRight->drawBox( window );
    }
  }
   
};


