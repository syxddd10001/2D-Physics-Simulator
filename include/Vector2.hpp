#pragma once
#include <utility>

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
    
    T operator* ( const Vector2& other ) const {
      return static_cast<T> (x * other.x + y * other.y ) ;
    }
    
    operator std::pair<T, T>( ) const {
      return std::make_pair( static_cast<T>(x), static_cast<T>(y) );
    }

  
  };
  
}