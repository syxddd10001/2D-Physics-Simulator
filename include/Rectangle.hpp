#pragma once
#include "Object.hpp"
#include <utility>

class Rectangle : public Object {
private:  
    std::pair<float, float> dimensions;
    sf::RectangleShape* shape;

public:
    Rectangle( float mass, float pos_x, float pos_y, float dim_x, float dim_y );
    Rectangle( );

    std::pair<float, float> getDimension( );
    sf::Shape* getShape( ) override;

    void setDimension( const std::pair<float, float> dim );
    void setPosition( const std::pair<float, float> pos ); 

    bool inBounds( const sf::Vector2f vector );

    bool mouseOnObject( const sf::Vector2f vector ) override;
};