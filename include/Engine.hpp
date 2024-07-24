#pragma once
#include <vector>

#include "Object.hpp"
#include "PhysicsMath.hpp"
#include "Physics.hpp"


class Engine{
protected:
    const float CREATION_INTERVAL = 0.2f;
    const float INTERRUPT_INTERVAL = 0.01f; 
    const float ZOOM_AMOUNT = 1.1f;

public:
    sf::RenderWindow* Window;
    sf::Clock clock;
    sf::View mainView;
    sf::View UIView;
    
    sf::Vector2i mousePos;
    sf::Vector2f mousePosf;
    
    
    float elapsed_time_spawn = CREATION_INTERVAL;
    float elapsed_time_move = INTERRUPT_INTERVAL;



    Engine( );
    ~Engine();
    void Update( const float* delta_time );
    void EventManager( );
    void addObject( Object* object );
    void zoomViewAt( sf::Vector2i pixel, float zoom );
    void collisionCheck( );
    void Render( );
    float getFramesPerSecond( );
    void UI( );
    void GetObjectsInArea( const point start, const point rect_size );
    void DragRectangle( );
    void objectDefault( );
};

