#pragma once
#include <SFML/Graphics.hpp>
#include <utility>
#include <iostream>
#include <memory>

#include "Vector2.hpp"
#include "AbstractBox.hpp"


using point = std::pair<float, float>;
using Vec2 = syxd::Vector2<float>;
const long double GRAVITATIONAL_CONSTANT = 6.674010551359 * pow(10,-11); // N m^2/kg^2
const float EPSILON = 0.00000001;
const float MY_PI = 3.14159265358979323846; // PI
const double MY_G_CONSTANT = 10; // GRAVITATIONAL CONSTANT OF (MY) UNIVERSE

class Object {
protected: 
  // Object State
  float m_mass;
  int m_object_id;
  
  Vec2 position_current; // position of the object
  Vec2 position_old;
  Vec2 velocity;
  Vec2 acceleration;
  Vec2 net_force;
  
  std::shared_ptr<sf::Shape> m_shape;
  std::shared_ptr<sf::Color> m_color;
  AbstractBox<float> m_queryBox;

public:    

  Object( float mass, float pos_x, float pos_y ) noexcept;
  Object( ) { };
  virtual ~Object( ) { };
  
  Vec2 getPosition() const;
  Vec2 getOldPosition() const;
  void setOldPosition( );
  virtual void setPosition( const Vec2 pos );

  void print_info( );
  virtual std::shared_ptr<sf::Shape> getShape();
  float getMass( );
  void setMass( const float mass );
  
  Vec2 getVelocity( ) const;
  void setVelocity( const Vec2 vel );
  
  Vec2 getAcceleration( ) const;
  void setAcceleration( const Vec2 acc );
  
  Vec2 getNetForce( ) const;
  void AddForce( const Vec2 force );
  
  virtual Vec2 getSize() const;
  virtual Vec2 getCenter() const;
  
  
  void setID( int id );
  int getID( );
  
  
  virtual bool mouseOnObject( const Vec2& vector );

  bool operator==( Object& other ) const {
    return ((std::abs(this->position_current.x- other.position_current.x) < EPSILON && 
    std::abs(this->position_current.y - other.position_current.y) < EPSILON )) && 
    (this->m_object_id == other.getID());
  };
  
  bool operator!=( Object& other ) const {
    return ((std::abs(this->position_current.x - other.position_current.x) >= EPSILON || 
    std::abs(this->position_current.y - other.position_current.y) >= EPSILON )) || 
    (this->m_object_id != other.getID());
  };
  
  void setQueryBox( AbstractBox<float> refbox );
  
  AbstractBox<float> getQueryBox( ) const;

  enum ObjectType {
      CIRCLE,
      RECTANGLE,
      NONE
  };
  
  static const std::map <std::string, ObjectType> m_object_type_map;
  
  virtual ObjectType getType();
  
  const bool operator= ( Object& other ) noexcept {
    return ( getID() == other.getID()) ;
  }
  
  void VerletIntegration( const float& delta_time );
  void Acceleration( const Vec2& acc );
  
  void applyForce( std::shared_ptr<Object> object, const Vec2& dir, const float& distance );
  void applyForce( const Vec2& force );
  
};