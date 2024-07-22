#include <Objects/Rectangle.hpp>
#include <Math/PhysicsMath.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <utility>

Rectangle::Rectangle(float mass, float pos_x, float pos_y, float dimX, float dimY)
: Object(mass, pos_x, pos_y), dimensions(dimX, dimY)
{
    shape = new sf::RectangleShape(sf::Vector2f(dimX, dimY));
    
    shape->setOutlineThickness(1.0f);
    shape->setOutlineColor(sf::Color::Black);
    shape->setFillColor(sf::Color::Blue);
}

Rectangle::Rectangle( ){


}

std::pair<float, float> Rectangle::getDimension( ){
    return  std::make_pair( this->dimensions.first, this->dimensions.second );
}

void Rectangle::setDimension( std::pair<float, float> dim ){
    this->dimensions.first = dim.first;
    this->dimensions.second = dim.second;
}

sf::Shape* Rectangle::getShape(){
    return shape;
}

void Rectangle::setPosition(std::pair<float, float> pos){
    position_x = pos.first;
    position_y = pos.second;
    shape->setPosition( sf::Vector2f( pos.first, pos.second ) );
}

bool Rectangle::inBounds( sf::Vector2f mousepos ) {   
  return ((mousepos.x >= position_x && mousepos.x <= position_x + dimensions.first) &&
          (mousepos.y >= position_y && mousepos.y <= position_y + dimensions.second));
}

bool Rectangle::mouseOnObject( sf::Vector2f vector ){
    return inBounds( vector );
}