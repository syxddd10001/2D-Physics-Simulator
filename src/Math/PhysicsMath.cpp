#include "PhysicsMath.hpp"

#define DEBUG 1
#if DEBUG == 1
#define DEBUG_PRINT(format, ...) printf(format, ##__VA_ARGS__)
#else
#define DEBUG_PRINT(format, ...) // Do nothing //
#endif


point pairAdd( const point& pair1, const point& pair2 ) {
  return point( pair1.first+pair2.first, pair1.second+pair2.second );
}    

point pairSubtract( const point& pair1, const point& pair2 ) {
  return point( pair1.first-pair2.first, pair1.second-pair2.second );
}

point pairMultiply( const point& pair1, const point& pair2 ) {
  return point( pair1.first*pair2.first, pair1.second*pair2.second );
}

float calculateDistance( const Vec2& pos1, const Vec2& pos2 ) {
  Vec2 difference = pos1 - pos2;
  return std::sqrt( difference.x * difference.x + difference.y * difference.y );
}

float dotProduct( const point& vectorA, const point& vectorB ) {
  return vectorA.first*vectorB.first + vectorA.second*vectorB.second;
}

Vec2 normalizeVector( const Vec2 p ) {
  float magnitude = sqrt( p.x * p.x + p.y * p.y );
  return ( (magnitude != 0) ? Vec2( p.x / magnitude, p.y / magnitude ) : Vec2( 0, 0 ) );
}

