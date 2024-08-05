#pragma once
#include "Object.hpp"
#include <utility>
#include <PhysicsMath.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>

class Rectangle : public Object {
private:  
  point dimensions;
  sf::RectangleShape* shape;

public:
  Rectangle( float mass, float pos_x, float pos_y, float dim_x, float dim_y );
  Rectangle( );

  point getDimension( );
  sf::Shape* getShape( ) override;

  void setDimension( const point dim );
  void setPosition( const point pos ); 

  bool inBounds( const sf::Vector2f vector );

  bool mouseOnObject( const sf::Vector2f vector ) override;
};