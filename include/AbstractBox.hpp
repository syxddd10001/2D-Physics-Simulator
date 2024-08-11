#pragma once
#include <utility>
#include <cmath>
#include <iostream>

#include "Vector2.hpp"
#include <SFML/Graphics.hpp>

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

  sf::RectangleShape* shape;

  AbstractBox( T Left = 0, T Top = 0, T Width = 0, T Height = 0 ) :
      left(Left), top(Top), width(Width), height(Height) {

  }

  AbstractBox( const Vec2 position, const Vec2 size ) :
      left(position.x), top(position.y), width(size.x), height(size.y) {
      shape = new sf::RectangleShape( sf::Vector2f { width, height } );
      shape->setPosition( sf::Vector2f( position ) );
      shape->setOutlineThickness ( 1.0f );
      shape->setOutlineColor( sf::Color::Blue );
      shape->setFillColor( sf::Color::Transparent );
  }

  AbstractBox( const point position, const point size )  :
      left(position.first), top(position.second), width(size.first), height(size.second) {

  }
  
  void setPosition( Vec2 posf, Vec2 size ){
    left = posf.x;
    top = posf.y;
    width = size.x;
    height = size.y;
    shape->setPosition( getTopLeft() );
  }


  T getRight( ) {
      return left + width;
  }

  T getBottom( ) {
      return top + height;
  }

  Vec2 getTopLeft( ) {
    return Vec2(left, top);
  }
  
  Vec2 getBottomRight(){
    return Vec2(getBottom(), getRight());
  }
  Vec2 getCenter( ) {
      return Vec2(left+(getSize().x / 2), top+(getSize().y / 2)); //
  }

  Vec2 getSize( ) {
      return Vec2( width, height );
  }

  bool contains( const AbstractBox<T>& box ) {
    return ( left <= box.left && box.getRight() <= getRight() &&
            top <= box.top && box.getBottom() <= getBottom() );
  }
  
  bool contains( Vec2 position, Vec2 size ) {
    Vec2 nearestPoint;
    nearestPoint.x = std::max(T(left), std::min(position.x, T(getRight())));
    nearestPoint.y = std::max(T(top), std::min(position.y, T(getBottom())));
    Vec2 RayToNearest = nearestPoint - position;
    T overlap = size.x - RayToNearest.magnitude();
    
    if ( std::isnan( overlap ) ) overlap = 0;
    
    if ( overlap >= 0 ){
      return true;
    } 
    
    return false;
  }

  bool intersects( AbstractBox<T> box ) {
      return !( left > box.getRight() || getRight() < box.left ||
              top > box.getBottom() || getBottom() < box.top );
  }
};