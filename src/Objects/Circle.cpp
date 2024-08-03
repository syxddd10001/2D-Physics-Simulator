#include "Circle.hpp"


Circle::Circle( float rad, float mass, float posX, float posY)
: Object( mass, posX, posY ), radius( rad )
{ 
    shape = new sf::CircleShape( radius );
    sf::Vector2f circleCenter( rad, rad );
    shape->setOrigin( circleCenter );
    shape->setOutlineThickness( 1.0f );
    shape->setFillColor( sf::Color::White );
    shape->setOutlineColor( sf::Color::Black );

}
Circle::Circle( ){

}

sf::Shape* Circle::getShape( ){
    return this->shape;
}


float Circle::setRadius( const float rad ){
    this->radius = rad;
    return radius;
}

float Circle::getRadius( ){
    return radius;
}

void Circle::setPosition( const std::pair<float, float> pos ){
    position_x = pos.first;
    position_y = pos.second;
    shape->setPosition( sf::Vector2f( position_x, position_y ) );
    
}

bool Circle::mouseOnObject( const sf::Vector2f vector ){
    return calculateDistance( getPosition(), std::pair<float, float> ( vector.x, vector.y ) ) < getRadius( );
}
