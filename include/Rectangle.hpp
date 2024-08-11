#pragma once
#include "Object.hpp"
#include <utility>
#include <PhysicsMath.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include "AbstractBox.hpp"
#include "Vector2.hpp"

class Rectangle : public Object {
private:  
  Vec2 dimensions;
  sf::RectangleShape* shape;

public:
  Rectangle( float mass, float pos_x, float pos_y, float dim_x, float dim_y );
  Rectangle( );

  Vec2 getDimension( );
  sf::Shape* getShape( ) override;

  void setDimension( const Vec2 dim );
  void setPosition( const Vec2 pos ); 

  bool inBounds( const sf::Vector2f vector );

  bool mouseOnObject( const sf::Vector2f vector ) override;
  
  Vec2 getSize( ) override;
  Vec2 getCenter( ) override;
  
  AbstractBox<float> getBoundingBox( );
  
  ObjectType getType() override {
    return ObjectType::RECTANGLE;
  }
};