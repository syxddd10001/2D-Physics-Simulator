#include <Objects/Circle.hpp>
#include <Math/PhysicsMath.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <utility>

Circle::Circle( float rad, float mass, float posX, float posY)
: Object( mass, posX, posY ), radius( rad )
{ 
    shape = new sf::CircleShape( radius );
    sf::Vector2f circleCenter( getRadius(), getRadius() );
    shape->setOrigin( circleCenter );
    shape->setOutlineThickness( 1.0f );
    shape->setFillColor( sf::Color::Black );
    shape->setOutlineColor( sf::Color::Black );

}
Circle::Circle( ){

}

sf::Shape* Circle::getShape( ){
    return shape;
}


float Circle::setRadius( float rad ){
    this->radius = rad;
    return radius;
}

float Circle::getRadius( ){
    return radius;
}

void Circle::setPosition( std::pair<float, float> pos ){
    position_x = pos.first;
    position_y = pos.second;
    shape->setPosition( sf::Vector2f( position_x, position_y ) );
    
}

bool Circle::mouseOnObject( sf::Vector2f vector ){
    float distance = calculateDistance( getPosition(), std::pair<float, float> ( vector.x, vector.y ) );

    if ( distance < getRadius( ) )
    {
        return true;
    }

    return false;

}
