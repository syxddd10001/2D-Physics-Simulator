#pragma once
#include <vector>

#include "Object.hpp"
#include "PhysicsMath.hpp"
#include "Physics.hpp"


class Engine{
public:
    sf::Clock clock;
    sf::Event evnt;
    std::vector<Object*> objects;
    float elapsed_time_spawn = creation_interval;
    float elapsed_time_move = interrupt_interval;

    float creation_interval = 0.2f;
    float interrupt_interval = 0.01f;
    float zoomAmount = 1.1f;

    const uint8_t frame_rate = 144;

    Object* p_selected_object = nullptr;
    sf::Vector2i mousePos;
    sf::Vector2f mousePosf;

    sf::Vector2f Center = sf::Vector2f(0.0f, 0.0f);
    sf::Vector2f HalfSize;

    sf::RenderWindow* Window;
    sf::View mainView;
    sf::View UIView;

    std::string spawn_type;
    float spawn_size = 50.0f;

    bool dragging = false;
    sf::Vector2i original_mouse_position;

    sf::Font font;

    sf::Vector2f mouseOnClickStart;
    sf::RectangleShape mouseDrawnBox;
    bool clicked = false;



    
    Engine( );
    ~Engine();
    void Update( float* delta_time );
    void EventManager( );
    void addObject( Object* object );
    void zoomViewAt( sf::Vector2i pixel, float zoom );
    void collisionCheck( );
    void Render( );
    float getFramesPerSecond( );
    void UI( );
    void DragRectangle( );
}; 