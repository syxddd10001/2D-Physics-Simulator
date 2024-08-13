#pragma once
#include <utility> // for std::pair
#include <cmath>
#include <memory>
#include "Circle.hpp"
#include "Rectangle.hpp"
#include "PhysicsMath.hpp" // for physics related math functions
#include "Vector2.hpp"

bool onCollision( std::shared_ptr<Circle> object1, std::shared_ptr<Circle> object2 );
void dynamicResponse ( std::shared_ptr<Circle> object1, std::shared_ptr<Circle> object2 );

#define EXPERIMENTAL 1
#if EXPERIMENTAL
  bool onCollision( std::shared_ptr<Circle> object1, std::shared_ptr<Rectangle> object2 );
  bool onCollision( std::shared_ptr<Rectangle> object1, std::shared_ptr<Rectangle> object2 ); 
#endif