#pragma once
#include <utility>
#include <cmath>

#include <SFML/Graphics.hpp>

namespace syxd {
  template <typename T>
  class Vector2{
  public:
    T x;
    T y;
    
    Vector2() : x(0), y(0) { }
    Vector2( T X, T Y ) : x(X), y(Y) { }
    
    Vector2 operator+ ( const Vector2& other ) const {
      return Vector2( x + other.x, y + other.y );
    }
    
    Vector2 operator- ( const Vector2& other ) const {
      return Vector2( x - other.x, y - other.y );
    }
    
    Vector2 operator* ( const T other ) const {
      return Vector2( x * other, y * other );
    }
    
    Vector2 operator/ ( const float other ) const {
      return Vector2( x /other, y /other );
    }
    
    Vector2& operator+=(const Vector2& other ) {
      x += other.x;
      y += other.y;
      return *this;
    }
    
    // dot product
    T operator* ( const Vector2& other ) const {
      return static_cast<T> (x * other.x + y * other.y ) ;
    }
    
    operator std::pair<T, T>( ) const {
      return std::make_pair( static_cast<T>(x), static_cast<T>(y) );
    }
    
    operator sf::Vector2f( ) const {
      return sf::Vector2f( (float) x, (float) y); 
    }
    
    explicit Vector2(const sf::Vector2f other) : x(static_cast<T>(other.x)), y(static_cast<T>(other.y)) {}
    explicit Vector2(const sf::Vector2u other) : x(static_cast<T>(other.x)), y(static_cast<T>(other.y)) {}
    explicit Vector2(const sf::Vector2i other) : x(static_cast<T>(other.x)), y(static_cast<T>(other.y)) {}

    T magnitude( ) const {
      return (T) std::sqrt(x*x+y*y);
    }
    
    Vector2<T> normalize( ) const {
      float mag = magnitude();
      return ( (mag != 0) ? Vector2<T>( x / mag, y / mag ) : Vector2<T>( 0, 0 ) );
    }
    
    
  
  };
  
}