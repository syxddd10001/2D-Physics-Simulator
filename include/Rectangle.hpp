#pragma once
#include "Object.hpp"
#include <utility>
#include <PhysicsMath.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <memory>
#include "AbstractBox.hpp"
#include "Vector2.hpp"

namespace syxd{
  class Rectangle : public Object {
  private:  
    Vec2 m_dimensions;
    std::shared_ptr<sf::RectangleShape> m_shape;
  
  public:
    Rectangle( float mass, float pos_x, float pos_y, float dim_x, float dim_y ) noexcept;
    Rectangle( );
    ~Rectangle() override {};
  
    Vec2 getDimension( );
    std::shared_ptr<sf::Shape> getShape( ) override;
  
    void setDimension( const Vec2 dim );
    void setPosition( const Vec2 pos ); 
  
    bool inBounds( const sf::Vector2f vector );
  
    bool mouseOnObject( const Vec2& vector ) override;
    
    Vec2 getSize( ) const override;
    Vec2 getCenter( ) const override;
    
    AbstractBox<float> getBoundingBox( ) const;
    
    ObjectType getType() override {
      return ObjectType::RECTANGLE;
    }
    
    bool intersects( std::shared_ptr<Rectangle> other ) const noexcept;
  
  };
}