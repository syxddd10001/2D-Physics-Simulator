#pragma once
#include <utility>
#include <math.h>
#include <iomanip>
#include <utility>
#include <cmath>
#include <iostream>
#include <memory>
#include <cassert>
#include "Object.hpp"
#include "Vector2.hpp"

using Vec2 = syxd::Vector2<float>;

point pairAdd( const point& pair1, const point& pair2 );
point pairSubtract( const point& pair1, const point& pair2 );
point pairMultiply( const point& pair1, const point& pair2 );

float calculateDistance( const Vec2& pos1, const Vec2& pos2 );

void verletIntegration( std::shared_ptr<Object> object, float delta_time, const Vec2 acceleration );

float dotProduct( const point& vectorA, const point& vectorB );
Vec2 applyForce( std::shared_ptr<Object> this_object );
Vec2 normalizeVector( const Vec2 p );

std::pair<float, Vec2> calculateCenterOfMass( const std::vector<std::shared_ptr<Object>> objects );
