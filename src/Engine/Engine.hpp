#pragma once
#include <vector>

#include <Objects/Object.hpp>
#include <Math/PhysicsMath.hpp>
#include <Engine/Physics.hpp>


class Engine{
public:
    sf::Clock clock;
    sf::Event evnt;
    std::vector<Object*> objects;
    float elapsed_time_spawn = 0.0f;
    float elapsed_time_move = 0.0f;

    const float creation_interval = 0.2f;
    const float interrupt_interval = 0.01f;
    const float zoomAmount = 1.1f;

    Object* p_selected_object = nullptr;
    sf::Vector2i mousePos;
    sf::Vector2f mousePosf;

    sf::Vector2f Center = sf::Vector2f(0.0f, 0.0f);
    sf::Vector2f HalfSize = sf::Vector2f(500.0f, 500.0f);

    sf::RenderWindow* Window;
    sf::View mainView;

    std::string spawn_type;

    bool dragging = false;
    sf::Vector2i original_mouse_position;

    Engine( );
    ~Engine();
    void Instance( );
    void Update( float* delta_time );
    void EventManager( );
    void InputManager( );
    void addObject( Object* object );
    void zoomViewAt( sf::Vector2i pixel, float zoom );
    void collisionCheck( );
    void Render( );
}; 