#pragma once
#include <vector>

#include <Objects/Object.hpp>
#include <Math/PhysicsMath.hpp>
#include <Engine/Physics.hpp>


class Engine{
protected:
    sf::RenderWindow* Window;
    sf::Clock clock;
    sf::Event evnt;
    std::vector<Object*> objects;
    float elapsed_time = 0.0f;
    float creation_interval = 0.15f;
    Object* p_selected_object = nullptr;
    sf::Vector2i mousePos;
    sf::Vector2f mousePosf;

public:

    Engine( );
    ~Engine();
    void Instance( );
    void Update( float* delta_time );
    void EventManager( );
    void InputManager( );
    void addObject( Object* object );
    void collisionCheck( );
    void Render( );
}; 