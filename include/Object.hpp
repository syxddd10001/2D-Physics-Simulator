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

namespace syxd{
  struct Color {
      sf::Uint8 r, g, b;
  };
  constexpr std::size_t color_count = 7;
  
  inline sf::Uint8 st_cast_ui8(float value) {
    return static_cast<sf::Uint8>(value);
  }

  class Object {
  protected: 
    // Object State
    float m_mass;
    int m_object_id;
    bool m_glow = false;

    Vec2 position_current; // position of the object
    Vec2 position_old;
    Vec2 velocity;
    Vec2 acceleration;
    Vec2 net_force;
    
    std::shared_ptr<sf::Shape> m_shape; // for regular
    std::shared_ptr<sf::Sprite> m_sprite = nullptr; // for glow


    sf::Shader* m_my_shader = nullptr;
    std::unique_ptr<sf::Color> m_color;
    AbstractBox<float> m_queryBox;
  
  public:    
  
    Object( float mass, float pos_x, float pos_y, bool glow ) noexcept;
    Object( ) { };
    virtual ~Object( ) = default;
    
    Vec2 getPosition() const;
    Vec2 getOldPosition() const;
    void setOldPosition( );
    void setOldPosition( const Vec2& pos );
    virtual void setPosition( const Vec2 pos );
    virtual void draw( std::shared_ptr<sf::RenderWindow> WINDOW ) = 0;
  
    void printInfo( );
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
    
    sf::Shader* getShader() const { 
      return m_my_shader; 
    }

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



    void setShader(sf::Shader* shader){
      m_my_shader = shader;
    }

    static const std::map <std::string, ObjectType> m_object_type_map;
    
    virtual ObjectType getType();
    virtual std::string typeName() const = 0;

    virtual std::string serializeCSV( int id ) const = 0;
    static std::unique_ptr<Object> deserializeCSV( const std::vector<std::string>& tokens );
    
    const bool operator= ( Object& other ) noexcept {
      return ( getID() == other.getID()) ;
    }
    
    void EulerIntegration( const float& delta_time );
    void VerletIntegration( const float& delta_time );
    
    void Acceleration( const Vec2& acc );
    
    void applyForce( std::shared_ptr<Object> object, const Vec2& dir, const float& distance );
    void applyForce( const Vec2& force );
    
    bool hasGlow();
    
  };
  
}