#pragma once
#include <vector>
#include <memory>

#include "Object.hpp"
#include "PhysicsMath.hpp"
#include "Physics.hpp"


class Engine{
protected:
    const float CREATION_INTERVAL = 0.2f;
    const float INTERRUPT_INTERVAL = 0.01f; 
    const float ZOOM_AMOUNT = 1.1f;
    float MOVE_SENSITIVITY = 1.f;
    float INPUT_INTERVAL = 0.1f;

public:
    std::shared_ptr<sf::RenderWindow> WINDOW;
    sf::Clock clock;
    sf::View mainView;
    sf::View UIView;
    
    sf::Vector2i mousePos;
    sf::Vector2f mousePosf;
    
    float elapsed_time_spawn = CREATION_INTERVAL;
    float elapsed_time_move = INTERRUPT_INTERVAL;
    float elapsed_time_input = INPUT_INTERVAL;


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
    void moveSelection( const sf::Vector2f delta );
    void checkObjectsSelected( );
    void moveAll( std::vector<Object*>* objects, const sf::Vector2f delta );
    void deleteObject( Object* object_to_delete, std::vector<Object*>& all_objects );
    void deleteSelectedObjects( std::vector<Object*>& all_objects );

};

