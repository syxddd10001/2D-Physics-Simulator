#pragma once
#include <utility>
#include <iostream>
#include <utility>

#include <SFML/Graphics.hpp>
#include "Object.hpp"
#include "PhysicsMath.hpp"
#include "AbstractBox.hpp"
#include "Vector2.hpp"

class Circle : public Object{
private: 
  float radius;
  sf::CircleShape* shape;

public:
  Circle( float rad, float mass, float pos_x, float pos_y );
  Circle( );
  float getRadius( );
  sf::Shape* getShape( ) override;
  float setRadius( const float rad );
  void setPosition( const Vec2 pos ) override;
  bool mouseOnObject( const sf::Vector2f vector ) override;
  Vec2 getSize( ) override;
  AbstractBox<float> getBoundingBox( );
  
  ObjectType getType() override {
    return ObjectType::CIRCLE;
  }
};