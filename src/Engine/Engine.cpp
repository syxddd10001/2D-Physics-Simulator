#include <vector>
#include <utility>
#include <thread>
#include <iostream>
#include <Objects/Object.hpp>
#include <Math/PhysicsMath.hpp>
#include "Engine.hpp"
#include <Engine/Physics.hpp>
#include <Objects/Rectangle.hpp>
Engine::Engine(  ){
    Window = new sf::RenderWindow ( sf::VideoMode( 1000, 1000 ), "2D Physics Simulator" );
    spawn_type = "cir";
    mainView = sf::View(sf::FloatRect(0, 0, 1500, 1500));
    Window->setView(mainView);

}

Engine::~Engine(){

}

void Engine::EventManager( ){
    if( Window->pollEvent( evnt ) ){
        switch( evnt.type ){
            case sf::Event::Closed:
                Window->close();
            break;
            
            case sf::Event::Resized:
                Window->setView( sf::View( sf::FloatRect( 0.0f, 0.0f, evnt.size.width, evnt.size.height ) ) );
            break;
           
            case sf::Event::MouseButtonPressed:
                if (evnt.mouseButton.button == sf::Mouse::Middle) dragging = true;
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

                if ( evnt.mouseButton.button == sf::Mouse::Left ){
                    p_selected_object = nullptr;
                }

                if ( evnt.mouseButton.button == sf::Mouse::Middle ) dragging = false;

                break;

            case sf::Event::KeyReleased:
                if (evnt.key.code == sf::Keyboard::Tab && elapsed_time_spawn >= interrupt_interval){
                    elapsed_time_spawn = 0.0f;   
        
                    spawn_type = (spawn_type == "cir") ? "rec" : "cir";
                
                    std::cout << spawn_type << std::endl;
                }
            break;

            case sf::Event::MouseWheelScrolled:
				if (evnt.mouseWheelScroll.delta > 0)
					zoomViewAt({ evnt.mouseWheelScroll.x, evnt.mouseWheelScroll.y }, (1.f / zoomAmount));
				else if (evnt.mouseWheelScroll.delta < 0)
					zoomViewAt({ evnt.mouseWheelScroll.x, evnt.mouseWheelScroll.y }, zoomAmount);
            
            break;

            case sf::Event::MouseMoved:
           
                const sf::Vector2i mouse_position{
                    evnt.mouseMove.x, evnt.mouseMove.y
                };

                if (dragging) {
                    const auto delta = 
                        Window->mapPixelToCoords(mouse_position) -
                        Window->mapPixelToCoords(original_mouse_position);


                    mainView.move(-delta);
                    Window->setView(mainView);
                }

                original_mouse_position = mouse_position;
            break;


         }
    }
}

void Engine::InputManager( ){

    if ( sf::Keyboard::isKeyPressed( sf::Keyboard::Y ) && elapsed_time_move >= interrupt_interval ) 
    {
        elapsed_time_move = 0.0f;   
        mainView.move(sf::Vector2f(-50.0f, 0.0f));
        Window->setView(mainView);

    }
    if ( sf::Keyboard::isKeyPressed( sf::Keyboard::U ) && elapsed_time_move >= interrupt_interval ) 
    {
        elapsed_time_move = 0.0f;   
        
        mainView.move(sf::Vector2f(50.0f, 0.0f));
        Window->setView(mainView);

    }

    if ( sf::Keyboard::isKeyPressed( sf::Keyboard::Space ) && elapsed_time_spawn >= creation_interval ){
        elapsed_time_spawn = 0.0f;   
        Object* obj = nullptr;
        
        if (spawn_type == "cir") {
            obj = new Circle( 50.0f, 2.0f, (float)mousePos.x, (float)mousePos.y );
            obj->setID(objects.size());
            
        }
        else {
            obj = new Rectangle( 50.0f, (float) mousePos.x, (float) mousePos.y, 50.0f, 50.0f);
            obj->setID(objects.size());
        }

        addObject( obj );      
    }

    if ( sf::Mouse::isButtonPressed( sf::Mouse::Right ) ) {
        if (!p_selected_object) { 
            for ( auto& selected : objects ) {
                if ( selected->mouseOnObject( mousePosf ) && p_selected_object == nullptr )
                {
                    p_selected_object = selected;
                    break;
                }
            }
        }
    }

    if ( sf::Mouse::isButtonPressed( sf::Mouse::Left ) ) {
        
        if ( !p_selected_object ) {
            for ( auto& selected : objects )
            {
                if ( selected->mouseOnObject( mousePosf ) && p_selected_object == nullptr )
                {
                    p_selected_object = selected;
                    break;
                }
            }
        }

        else {
            mousePosf = Window->mapPixelToCoords(sf::Mouse::getPosition(*Window));
            
            point mouse ( mousePosf.x, mousePosf.y );
            p_selected_object->setPosition( mouse );
            p_selected_object->setVelocity( point ( 0.0f, 0.0f ) );
            p_selected_object = nullptr;

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
    if ( p_selected_object != nullptr && sf::Mouse::isButtonPressed( sf::Mouse::Right ) )
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

void Engine::zoomViewAt( sf::Vector2i pixel, float zoom )
{
	const sf::Vector2f beforeCoord{ Window->mapPixelToCoords(pixel) };
	mainView.zoom(zoom);
	Window->setView(mainView);
	const sf::Vector2f afterCoord{ Window->mapPixelToCoords(pixel) };
	const sf::Vector2f offsetCoords{ beforeCoord - afterCoord };
	mainView.move(offsetCoords);
	Window->setView(mainView);
}