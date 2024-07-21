#include <vector>
#include <utility>
#include <thread>

#include <Objects/Object.hpp>
#include <Math/PhysicsMath.hpp>
#include "Engine.hpp"
#include <Engine/Physics.hpp>

Engine::Engine(  ){
    Window = new sf::RenderWindow ( sf::VideoMode( 1000, 1000 ), "2D Physics Simulator" );
}

Engine::~Engine(){

}



void Engine::Instance( ){
    while ( Window->isOpen() ){
        // spaced in terms of threads (coming later)
        Window->clear();   
        
        mousePos = sf::Mouse::getPosition( *Window );
        mousePosf = { static_cast<float>( mousePos.x ), static_cast<float>( mousePos.y ) };

        float delta_time = clock.restart().asSeconds();
        elapsed_time += delta_time;

        EventManager( ); 
        
        InputManager( );

        Render();

        
        
        collisionCheck( );

        Update( &delta_time );
           
        
        
        Window->display();
    }
}

void Engine::EventManager( ){
    while( Window->pollEvent( evnt ) ){
        switch( evnt.type ){
            case sf::Event::Closed:
                Window->close();
                break;
            
            case sf::Event::Resized:
                Window->setView( sf::View( sf::FloatRect( 0.0f, 0.0f, evnt.size.width, evnt.size.height ) ) );
                break;
           
            case sf::Event::MouseButtonReleased:
                if (evnt.mouseButton.button == sf::Mouse::Right)
                {
                    if (p_selected_object != nullptr)
                    {     
                        float launch_speed = 3.5f;
                        point position ( p_selected_object->getPosition() ); 
                        float velocity_x = launch_speed * ( position.first - mousePosf.x );
                        float velocity_y = launch_speed * ( position.second - mousePosf.y );
                        p_selected_object->setVelocity( point( velocity_x, velocity_y ) );
                        p_selected_object = nullptr;
                    }

                    break;
                }
         }
    }
}

void Engine::InputManager( ){

    if ( sf::Keyboard::isKeyPressed( sf::Keyboard::Space ) && elapsed_time >= creation_interval ){
        elapsed_time = 0.0f;   
        Circle* object_x = new Circle( 50.0f, 2.0f, (float)mousePos.x, (float)mousePos.y );
        addObject( object_x );      
    }

    if ( sf::Mouse::isButtonPressed( sf::Mouse::Right ) ) {
        for ( auto& selected : objects )
        {
            if ( selected->mouseOnObject( mousePosf ) && p_selected_object == nullptr )
            {
                p_selected_object = selected;
                break;
            }
        }
    }

}

void Engine::Update( float* delta_time ){
    for ( int i = 0; i < objects.size(); i++ ) {
            point pos_f = calculateVelocity( objects[i], *delta_time );
            objects[i]->setPosition( pos_f );
            sf::Shape* sh = objects[i]->getShape();
            Window->draw( *sh );     
    }  
}

void Engine::addObject( Object* object ) {
    objects.push_back( object );
}

void Engine::collisionCheck( ){
    for ( auto& current : objects )
    {
        for ( auto& other : objects )
        {     
            if ( current != other && ( typeid( *current ) == typeid( Circle ) && typeid( *other ) == typeid( Circle ) ) )
            {    
                if ( onCollision( dynamic_cast<Circle*>( current ), dynamic_cast<Circle*>( other ) ) )
                {
                    dynamicResponse( dynamic_cast<Circle*>( current ), dynamic_cast<Circle*>( other ) );
                }
            }

            /*else if ( ( typeid( *current ) == typeid( Rectangle ) && typeid( *other ) == typeid( Circle ) ) )
            {    
                if ( onCollision( dynamic_cast<Circle*>(current), dynamic_cast<Circle*>(other) ) )
                {
                    // collision response
                }
            }*/
        }
    }
}

void Engine::Render( ){
    if ( p_selected_object != nullptr )
    {
        std::pair<float, float> position( p_selected_object->getPosition() );
        float distance = calculateDistance( position, std::pair<float, float> ( mousePosf.x, mousePosf.y ) );
        if ( distance > 500.0f )
        {
            p_selected_object = nullptr;
        }
        sf::VertexArray line( sf::Lines, 10 );
        
        line[0].position = sf::Vector2f( position.first, position.second );
        line[0].color = sf::Color::Green;
        line[1].position = mousePosf;
        line[1].color = sf::Color::Green;
        Window->draw( line );

    }
}