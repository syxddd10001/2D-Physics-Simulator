#pragma once
#include <SFML/Graphics.hpp>
#include <utility>

class Object {
protected: 
    float position_x, position_y; // position of the center of the object
    float velocity_x, velocity_y;
    float acceleration_x, acceleration_y;
    float mass;
    int object_id;
    sf::Shape* shape;
    

public:
    Object( float mass, float pos_x, float pos_y );
    Object( );
    virtual ~Object( ) = 0;
    
    std::pair<float, float> getPosition();

    virtual void setPosition( std::pair<float, float> pos );

    void print_info( );
    virtual sf::Shape* getShape();
    float getMass( );
    void setMass( float mass );
    
    std::pair<float, float> getVelocity( );
    std::pair<float, float> setVelocity( float vel_x, float vel_y );
    void setVelocity( std::pair<float, float> vel );
    
    std::pair<float, float> getAcceleration( );
    std::pair<float, float> setAcceleration( float acc_x, float acc_y );
    void setAcceleration( std::pair<float, float> acc );

    void setID( int id );
    int getID( );

    virtual bool mouseOnObject( sf::Vector2f vector );

};