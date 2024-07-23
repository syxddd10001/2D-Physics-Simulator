#pragma once

#include "Circle.hpp"
#include "Rectangle.hpp"
#include <utility> // for std::pair


bool onCollision( Circle* object1, Circle* object2 );
void dynamicResponse ( Circle* object1, Circle* object2 );

#define EXPERIMENTAL 0
#if EXPERIMENTAL
    bool onCollision( Circle* object1, Rectangle object2 );
#endif