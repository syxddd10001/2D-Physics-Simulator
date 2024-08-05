#pragma once
#include <SFML/Graphics.hpp>
#include <utility>
#include <iostream>

using point = std::pair<float, float>;

class Object {
protected: 
  // Object State
  float position_x, position_y; // position of the center of the object
  float velocity_x, velocity_y;
  float acceleration_x, acceleration_y;
  float force_x, force_y;
  float mass;
  int object_id;
  sf::Shape* shape;
  sf::Color* color;

public:    
  Object( float mass, float pos_x, float pos_y );
  Object( );
  virtual ~Object( ) = 0;
  
  point getPosition();

  virtual void setPosition( const point pos );

  void print_info( );
  virtual sf::Shape* getShape();
  float getMass( );
  void setMass( const float mass );
  
  point getVelocity( );
  point setVelocity( const float vel_x, const float vel_y );
  void setVelocity( const point vel );
  
  point getAcceleration( );
  point setAcceleration( const float acc_x, const float acc_y );
  void setAcceleration( const point acc );
  
  void setID( int id );
  int getID( );

  virtual bool mouseOnObject( const sf::Vector2f vector );

  enum ObjectType {
      CIRCLE,
      RECTANGLE
  };

};