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

    void setDimension( std::pair<float, float> dim );
    void setPosition( std::pair<float, float> pos ); 

    bool inBounds( sf::Vector2f vector );

    bool mouseOnObject( sf::Vector2f vector ) override;
};