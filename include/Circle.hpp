#pragma once
#include <utility>
#include <iostream>
#include <memory>
#include <cstdlib>

#include <SFML/Graphics.hpp>
#include "Object.hpp"
#include "PhysicsMath.hpp"
#include "AbstractBox.hpp"
#include "Vector2.hpp"

namespace syxd{
  using namespace syxd;
  
  class Circle : public Object{
  private: 
    float m_radius;
    std::shared_ptr<sf::CircleShape> m_shape;
  
  public:
    Circle( float rad, float mass, float pos_x, float pos_y ) noexcept;
    ~Circle() override {};
    Circle( );
    float getRadius( );
    std::shared_ptr<sf::Shape> getShape( ) override;
    float setRadius( const float rad );
    void setPosition( const Vec2 pos ) override;
    bool mouseOnObject( const Vec2& vector ) override;
    Vec2 getSize( ) const override;
    Vec2 getCenter( ) const override;
    AbstractBox<float> getBoundingBox( ) const;
    
    ObjectType getType() override {
      return ObjectType::CIRCLE;
    }
  };
}