#pragma once
#include <utils.hpp>
#include <SFML/Graphics.hpp>
#include <Vector2.hpp>
#include <memory> 
#include <array>
#include <string>
#include <iostream>
#include <stack>
#include <algorithm>
#include <functional>

namespace syxd{
  using namespace syxd;
  
  class UI_Element {
  private:
    uint32_t m_id;
    std::string m_identifier;
    sf::Vector2f m_position;
    sf::Vector2f m_size;
    bool m_hidden = false;
    std::string m_type;

  public:
    UI_Element ( const uint32_t id, const std::string identifier, const sf::Vector2f position ) noexcept;
    UI_Element ( ) { };
    virtual ~UI_Element ( ) = default;
    
    const uint32_t getID(){
      return m_id;
    } 

    const std::string getIdentifier(){
      return m_identifier;
    } 

    void setSize( const sf::Vector2f& size ){
      m_size = size;
    }
    void setSize( const float& size_x, const float& size_y ) {
      m_size = { size_x, size_y };
    }
    void setSize( const Vector2<float>& size ) {
      m_size = size;
    }
  
    void setPosition( const sf::Vector2f& position ) {
      m_position = position;
      
    }
    void setPosition( const float& pos_x, const float& pos_y ) {
      m_position = { pos_x, pos_y };
    }
    void setPosition( const Vector2<float>& position ) {
      m_size = position;
    }

    sf::Vector2f getPosition( ) {
      return m_position;
    }
    
    void hide( const bool& b ){
      m_hidden = b;
    }

    bool hidden( ){
      return m_hidden;
    }

    std::string what_type( ){
      return m_type;
    };

    virtual void render( sf::RenderWindow* WINDOW_REF, const float& delta_time ) = 0;

  };

}