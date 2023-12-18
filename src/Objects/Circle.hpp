#pragma once
#include "Object.hpp"
#include <utility>

class Circle : public Object{
private: 
    float radius;
    sf::CircleShape* shape;

public:
    Circle( float rad, float mass, float posX, float posY );
    Circle( );
    float getRadius( );
    sf::Shape* getShape( ) override; 
    float setRadius( float rad );
    void setPosition( std::pair<float, float> pos ) override;
    
    bool mouseOnObject( sf::Vector2f vector ) override;
};