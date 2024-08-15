#pragma once
#include <SFML/Graphics.hpp>
#include <utility>
#include <iostream>
#include <memory>

#include "Vector2.hpp"
#include "AbstractBox.hpp"

#define EPSILON 0.00000001

using point = std::pair<float, float>;
using Vec2 = syxd::Vector2<float>;

class Object {
protected: 
  // Object State
  float position_x, position_y; // position of the center of the object
  float velocity_x, velocity_y;
  float acceleration_x, acceleration_y;
  float force_x, force_y;
  float mass;
  int object_id;
  std::shared_ptr<sf::Shape> shape;
  std::shared_ptr<sf::Color> color;
  AbstractBox<float> queryBox;

public:    
  Object( float mass, float pos_x, float pos_y );
  Object( ) { };
  virtual ~Object( ) { };
  
  Vec2 getPosition() const;

  virtual void setPosition( const Vec2 pos );

  void print_info( );
  virtual std::shared_ptr<sf::Shape> getShape();
  float getMass( );
  void setMass( const float mass );
  
  Vec2 getVelocity( ) const ;
  Vec2 setVelocity( const float vel_x, const float vel_y );
  void setVelocity( const Vec2 vel );
  
  Vec2 getAcceleration( ) const;
  Vec2 setAcceleration( const float acc_x, const float acc_y );
  void setAcceleration( const Vec2 acc );
  
  virtual Vec2 getSize() const;
  virtual Vec2 getCenter() const;
  
  
  void setID( int id );
  int getID( );
  
  
  virtual bool mouseOnObject( const Vec2& vector );

  bool operator==( Object& other ) const {
    return ((std::abs(this->position_x - other.position_x) < EPSILON && std::abs(this->position_y - other.position_y) < EPSILON )) && 
    (this->object_id == other.getID());
  };
  
  bool operator!=( Object& other ) const {
    return ((std::abs(this->position_x - other.position_x) >= EPSILON || std::abs(this->position_y - other.position_y) >= EPSILON )) || 
    (this->object_id != other.getID());
  };
  
  void setQueryBox( AbstractBox<float> refbox );
  
  AbstractBox<float> getQueryBox( ) const;

  enum ObjectType {
      CIRCLE,
      RECTANGLE,
      NONE
  };
  
  virtual ObjectType getType();
  
  const bool operator= ( Object& other ) noexcept {
    return (getID() == other.getID());
  }

};