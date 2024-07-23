#pragma once
#include <vector>

#include "Object.hpp"
#include "PhysicsMath.hpp"
#include "Physics.hpp"


class Engine{
protected:
    sf::Event evnt;
    std::vector<Object*> objects;
    std::vector<Object*> selected_objects;

    float creation_interval = 0.2f;
    float interrupt_interval = 0.01f;
    float zoomAmount = 1.1f;

    Object* p_selected_object = nullptr;
    sf::Vector2f mousePos_prev;
    sf::Vector2i mousePos_prev_all;


    sf::Vector2f Center = sf::Vector2f( 0.0f, 0.0f );
    sf::Vector2f HalfSize;

    std::string spawn_type;
    float spawn_size = 50.0f;

    bool dragging = false;
    sf::Vector2i original_mouse_position;

    sf::Font font;

    sf::Vector2f mouseOnClickStart;
    sf::RectangleShape mouseDrawnBox;
    bool clicked = false;
    
    bool mouseonobj = false;
    bool selection_stop = false;
    bool select_mode = false;

public:
    sf::RenderWindow* Window;
    sf::Clock clock;
    sf::View mainView;
    sf::View UIView;
    
    sf::Vector2i mousePos;
    sf::Vector2f mousePosf;

    float elapsed_time_spawn = creation_interval;
    float elapsed_time_move = interrupt_interval;

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
}; 