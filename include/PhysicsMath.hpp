#pragma once
#include <utility>
#include <math.h>
#include <iomanip>
#include <utility>
#include <cmath>
#include <iostream>
#include <memory>
#include <cassert>
#include <tuple>
#include "Object.hpp"
#include "Vector2.hpp"

using namespace syxd;

using Vec2 = syxd::Vector2<float>;

point pairAdd( const point& pair1, const point& pair2 );
point pairSubtract( const point& pair1, const point& pair2 );
point pairMultiply( const point& pair1, const point& pair2 );
float calculateDistance( const Vec2& pos1, const Vec2& pos2 );
float dotProduct( const point& vectorA, const point& vectorB );
Vec2 normalizeVector( const Vec2 p );