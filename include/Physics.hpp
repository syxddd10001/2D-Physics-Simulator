#pragma once
#include <cmath>
#include <memory>

#include "Vector2.hpp"
#include "PhysicsMath.hpp" // for physics related math functions
#include "Circle.hpp"
#include "Rectangle.hpp"

bool onCollision( std::shared_ptr<Circle> object1, std::shared_ptr<Circle> object2 );
void dynamicResponse ( std::shared_ptr<Circle> object1, std::shared_ptr<Circle> object2 );

bool onCollision( std::shared_ptr<Circle> object1, std::shared_ptr<Rectangle> object2 );
void dynamicResponse( std::shared_ptr<Circle> object1, std::shared_ptr<Rectangle> object2 ) ;

bool onCollision( std::shared_ptr<Rectangle> object1, std::shared_ptr<Rectangle> object2 ); 
void dynamicResponse( std::shared_ptr<Rectangle> object1, std::shared_ptr<Rectangle> object2 ) ;