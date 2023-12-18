#pragma once

#include <Objects/Circle.hpp>
#include <utility> // for std::pair


bool onCollision( Circle* object1, Circle* object2 );
void dynamicResponse ( Circle* object1, Circle* object2 );
