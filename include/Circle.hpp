#pragma once
#include <utility>
#include <iostream>
#include <memory>
#include <SFML/Graphics.hpp>
#include "Object.hpp"
#include "PhysicsMath.hpp"
#include "AbstractBox.hpp"
#include "Vector2.hpp"

class Circle : public Object{
private: 
  float radius;
  std::shared_ptr<sf::CircleShape> shape;

public:
  Circle( float rad, float mass, float pos_x, float pos_y );
  Circle( );
  float getRadius( );
  std::shared_ptr<sf::Shape> getShape( ) override;
  float setRadius( const float rad );
  void setPosition( const Vec2 pos ) override;
  bool mouseOnObject( const Vec2 vector ) override;
  Vec2 getSize( ) const override;
  Vec2 getCenter( ) const override;
  AbstractBox<float> getBoundingBox( ) const;
  
  
  ObjectType getType() override {
    return ObjectType::CIRCLE;
  }
};