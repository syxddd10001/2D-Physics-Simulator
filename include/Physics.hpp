#pragma once
#include <utility> // for std::pair
#include <cmath>

#include "Circle.hpp"
#include "Rectangle.hpp"
#include "PhysicsMath.hpp" // for physics related math functions
#include "Vector2.hpp"

bool onCollision( Circle* object1, Circle* object2 );
void dynamicResponse ( Circle* object1, Circle* object2 );

#define EXPERIMENTAL 0
#if EXPERIMENTAL
  bool onCollision( Circle* object1, Rectangle object2 );
  bool onCollision( Rectangle * object1, Rectangle* object2 ); 
#endif