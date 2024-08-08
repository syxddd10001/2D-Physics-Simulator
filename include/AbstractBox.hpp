#pragma once
#include <utility>
#include "Object.hpp"
#include "Vector2.hpp"

using point = std::pair<float, float>;
using Vec2 = syxd::Vector2<float>;

template<typename T>
class AbstractBox
{
public:
  T left;
  T top;
  T width;
  T height; 

  constexpr AbstractBox( T Left = 0, T Top = 0, T Width = 0, T Height = 0 ) noexcept :
      left(Left), top(Top), width(Width), height(Height) {

  }

  constexpr AbstractBox( const Vec2 position, const Vec2 size ) noexcept :
      left(position.x), top(position.y), width(size.x), height(size.y) {

  }

  constexpr AbstractBox( const point position, const point size ) noexcept :
      left(position.first), top(position.second), width(size.first), height(size.second) {

  }

  constexpr T getRight( ) const noexcept {
      return left + width;
  }

  constexpr T getBottom( ) const noexcept {
      return top + height;
  }

  constexpr Vec2 getTopLeft( ) const noexcept {
      return Vec2(left, top);
  }

  constexpr Vec2 getCenter( ) const noexcept {
      return Vec2((left + width) / 2, (top + height) / 2);
  }

  constexpr Vec2 getSize( ) const noexcept {
      return Vec2( width, height );
  }

  constexpr bool contains( const AbstractBox<T>& box ) const noexcept {
    return ( left <= box.left && box.getRight() <= getRight() &&
            top <= box.top && box.getBottom() <= getBottom() );
  }
  
  bool contains( point& position, point& size ) const noexcept {
    return ( position.first-size.first >= getTopLeft().x && position.second <= getTopLeft().y
            && position.second + size.second >= getBottom() && position.first + size.first <= getRight() );
  }
  
  bool contains( Vec2& position, Vec2& size ) const noexcept {
    return ( position.x-size.x >= getTopLeft().x && position.y <= getTopLeft().y
            && position.y + size.y >= getBottom() && position.x + size.x <= getRight() );
  }

  constexpr bool intersects( const AbstractBox<T>& box ) const noexcept {
      return !( left >= box.getRight() || getRight() <= box.left ||
              top >= box.getBottom() || getBottom() <= box.top );
  }
};