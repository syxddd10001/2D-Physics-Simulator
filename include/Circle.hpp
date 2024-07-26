#pragma once
#include "Object.hpp"
#include <utility>

class Circle : public Object{
private: 
    float radius;
    sf::CircleShape* shape;

public:
    Circle( float rad, float mass, float pos_x, float pos_y );
    Circle( );
    float getRadius( );
    sf::Shape* getShape( ) override; 
    float setRadius( const float rad );
    void setPosition( const std::pair<float, float> pos ) override;
    bool mouseOnObject( const sf::Vector2f vector ) override;
};