#include <vector>
#include <utility>
#include <thread>
#include <iostream>
#include <Object.hpp>
#include <PhysicsMath.hpp>
#include "Engine.hpp"
#include <Physics.hpp>
#include <Rectangle.hpp>
#include <cmath>
#define FRAME_RATE 144

sf::Event evnt;
std::vector<Object*> objects;
std::vector<Object*> selected_objects;

Object* p_selected_object = nullptr;
sf::Vector2f mousePos_prev;
sf::Vector2f mousePos_prev_all;


sf::Vector2f Center = sf::Vector2f( 0.0f, 0.0f );
sf::Vector2f HalfSize;

std::string spawn_type;
float spawn_size = 50.0f;


sf::Vector2i original_mouse_position;

sf::Font font;

sf::Vector2f mouseOnClickStart;
sf::RectangleShape mouseDrawnBox;


bool clicked = false;
bool dragging = false;
bool mouseonobj = false;
bool selection_stop = false;
bool select_mode = false;

sf::Vector2i originalCoordinates;
bool focus = false;

Engine::Engine( ){
    Window = new sf::RenderWindow ( sf::VideoMode( 1000, 1000 ), "2D Physics Simulator" );
    HalfSize = sf::Vector2f( Window->getSize().x/2, Window->getSize().y/2 );
    if ( !font.loadFromFile( "static/fonts/Silver.ttf" ) ){
        std::cout << "No such file\n"; 
    }
    spawn_type = "cir";
    mainView = sf::View( sf::FloatRect( 0, 0, Window->getSize().x, Window->getSize().y ) );
    
    originalCoordinates = Window->mapCoordsToPixel(sf::Vector2f{ ((float)(Window->getSize().x)/2), ((float)(Window->getSize().y)/2)});
    std::cout << originalCoordinates.x << "," << originalCoordinates.y << "\n";
    Window->setFramerateLimit( FRAME_RATE );
    Window->setView( mainView );
}

Engine::~Engine( ){

}

void Engine::EventManager( ){
    if( Window->pollEvent( evnt ) ){
        switch( evnt.type ){
            case sf::Event::Closed:
                Window->close();
            break;
            
            case sf::Event::Resized:
                Window->setView( mainView = sf::View( sf::FloatRect( 0.0f, 0.0f, evnt.size.width, evnt.size.height ) ) );
                Window->setView( UIView = sf::View( sf::FloatRect( 0.0f, 0.0f, evnt.size.width, evnt.size.height ) ) );
            break;
           
            case sf::Event::MouseButtonPressed:
                if ( evnt.mouseButton.button == sf::Mouse::Middle ) dragging = true;

                if ( evnt.mouseButton.button ==  sf::Mouse::Right ) {
                    
                    if ( !p_selected_object ) { 
                        for ( auto& selected : objects ) {
                            if ( selected->mouseOnObject( mousePosf ) ) 
                            {
                                p_selected_object = selected;                             
                                break;
                            }
                        }
                    }
                }

                if ( evnt.mouseButton.button == sf::Mouse::Left && !select_mode ) {
                    
                    if ( !p_selected_object ) {
                        for ( auto& selected : objects )
                        {
                            if ( selected->mouseOnObject( mousePosf ) )
                            {
                                p_selected_object = selected;
                                break;
                            }
                        }
                    }

                    else {
                        p_selected_object->getShape()->setOutlineColor( sf::Color::Red );
                        p_selected_object->getShape()->setOutlineThickness( 3.0f );                                  
                    }
      

                }

               
                
            break;

            case sf::Event::MouseButtonReleased:
                if ( evnt.mouseButton.button == sf::Mouse::Right )
                {
                    if ( p_selected_object != nullptr )
                    {     
                        float launch_speed = 3.5f;
                        point position ( p_selected_object->getPosition() ); 
                        float velocity_x = launch_speed * ( position.first - mousePosf.x );
                        float velocity_y = launch_speed * ( position.second - mousePosf.y );
                        p_selected_object->setVelocity( point( velocity_x, velocity_y ) );
                        
                        sf::Shape* sh = p_selected_object->getShape();
                        sh->setOutlineColor(sf::Color::Black);
      
                        p_selected_object = nullptr;
                    }

                    

                    break;
                }

                if ( evnt.mouseButton.button == sf::Mouse::Left ){
                    clicked = false;
                    mouseDrawnBox.setSize(sf::Vector2f({0.0f,0.0f}));

                    if ( p_selected_object ){
                        sf::Shape* sh = p_selected_object->getShape();
                        sh->setOutlineColor( sf::Color::Black );

                        p_selected_object = nullptr;
                    }

                    if ( select_mode && selected_objects.size() > 0 ) {
                        selection_stop = true;
                    }
                }

                if ( evnt.mouseButton.button == sf::Mouse::Middle ) dragging = false;

                break;

            case sf::Event::KeyReleased:
                if ( evnt.key.code == sf::Keyboard::F && elapsed_time_spawn >= INTERRUPT_INTERVAL ){
                    elapsed_time_spawn = 0.0f;  
                    focus = true;
                }
                
                if ( evnt.key.code == sf::Keyboard::Tab && elapsed_time_spawn >= INTERRUPT_INTERVAL ){
                    elapsed_time_spawn = 0.0f;   
        
                    spawn_type = ( spawn_type == "cir" ) ? "rec" : "cir";
                
                    std::cout << spawn_type << std::endl;
                }

                if ( evnt.key.code == sf::Keyboard::T && elapsed_time_spawn >= CREATION_INTERVAL ) 
                {
                    elapsed_time_spawn = 0.0f;   
                    spawn_size += 10.0f;
                    std::cout << spawn_size << std::endl;
                }
                if ( evnt.key.code == sf::Keyboard::Y && elapsed_time_spawn >= CREATION_INTERVAL ) 
                {
                    elapsed_time_spawn = 0.0f;   
                    spawn_size -= 10.0f;
                    std::cout << spawn_size << std::endl;

                }

                if ( evnt.key.code == sf::Keyboard::S ){
                    select_mode = !select_mode;
                    objectDefault( );
                }
                if ( evnt.key.code == sf::Keyboard::Escape ){
                    objectDefault( );

                }



            break;

            case sf::Event::KeyPressed:
                if ( evnt.key.code == sf::Keyboard::Space && elapsed_time_spawn >= CREATION_INTERVAL ){
                    elapsed_time_spawn = 0.0f;   
                    Object* obj = nullptr;
                    
                    if ( spawn_type == "cir" ) {
                        float t_mass = spawn_size*10.0f;
                        obj = new Circle( spawn_size, t_mass, mousePosf.x, mousePosf.y );
                        obj->setID( objects.size() );
                        
                    }
                    else if (spawn_type == "rec") {
                        float t_mass = spawn_size*10.0f;
                        obj = new Rectangle( t_mass, mousePosf.x,  mousePosf.y, spawn_size, spawn_size );
                        obj->setID( objects.size() );
                    }
                    std::cout << obj << std::endl; 
                    addObject( obj );
                }


                
            break;

            case sf::Event::MouseWheelScrolled:
				if ( evnt.mouseWheelScroll.delta > 0 )
					zoomViewAt({ evnt.mouseWheelScroll.x, evnt.mouseWheelScroll.y }, (1.f / ZOOM_AMOUNT));
				else if ( evnt.mouseWheelScroll.delta < 0 )
					zoomViewAt({ evnt.mouseWheelScroll.x, evnt.mouseWheelScroll.y }, ZOOM_AMOUNT);
            
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
    if ( !p_selected_object ){
        mousePos_prev = Window->mapPixelToCoords( sf::Mouse::getPosition( *Window)  );    
    }


    if ( sf::Event::MouseMoved && sf::Mouse::isButtonPressed( sf::Mouse::Left ) && p_selected_object ){
        point delta ( mousePosf.x-mousePos_prev.x, mousePosf.y-mousePos_prev.y );
        point curr_pos { p_selected_object->getPosition().first+delta.first, p_selected_object->getPosition().second+delta.second };
        p_selected_object->setPosition( curr_pos );
        p_selected_object->setVelocity( point ( 0.0f, 0.0f ) );
        mousePos_prev = mousePosf;
    }

    #define EXPERIMENTAL_1 0
    #if EXPERIMENTAL_1
    if ( sf::Event::MouseMoved && sf::Mouse::isButtonPressed( sf::Mouse::Left ) && !p_selected_object && select_mode && selected_objects.size() != 0) {
        point delta ( mousePosf.x-mousePos_prev_all.x, mousePosf.y-mousePos_prev_all.y );
        Object* temp;
        for ( auto& selected : selected_objects )
        {
            mouseonobj = false;
            if ( selected->mouseOnObject( mousePosf ) ){
                mouseonobj = true;
                temp = selected;
                break;
            }
        }
        
        if ( mouseonobj && selection_stop ){
            
            point curr_pos { 
                temp->getPosition().first+delta.first, 
                temp->getPosition().second+delta.second };

            
            temp->setPosition( curr_pos );
            temp->setVelocity( point ( 0.0f, 0.0f ) );
        }

        mousePos_prev_all = mousePosf;


    }
    #endif
                    
}

void Engine::objectDefault(){
    for ( auto* obj : objects ){
        obj->getShape()->setOutlineColor(sf::Color::Black);
    }
    selection_stop = false;
    selected_objects.clear();
    selected_objects.shrink_to_fit();
}


void Engine::GetObjectsInArea( const point start, const point rect_size ){
    for ( auto* obj : objects ){
        point pos = obj->getPosition();
        if ( ( pos.first >= start.first && pos.first <= ( start.first+rect_size.first ) ) && 
             ( pos.second >= start.second && pos.second <= ( start.second+rect_size.second ) ) ){
            selected_objects.push_back( obj );
            obj->getShape()->setOutlineColor( sf::Color::Red );
        }
    }

    selected_objects.shrink_to_fit();
}


void Engine::DragRectangle( ){
    if ( !select_mode || selection_stop ) return ;

    if ( sf::Event::MouseMoved && sf::Mouse::isButtonPressed( sf::Mouse::Left ) && clicked && !p_selected_object ){
        
        sf::Vector2f rect_size( mousePosf.x - mouseOnClickStart.x, mousePosf.y - mouseOnClickStart.y );
        mouseDrawnBox.setPosition( mouseOnClickStart.x, mouseOnClickStart.y );
        mouseDrawnBox.setOutlineColor( sf::Color::White );
        mouseDrawnBox.setOutlineThickness(1.0f);
        mouseDrawnBox.setSize( rect_size );
        GetObjectsInArea( point( mouseOnClickStart.x, mouseOnClickStart.y ), point( rect_size.x,rect_size.y ) );
        Window->draw( mouseDrawnBox );
    }


    if ( sf::Mouse::isButtonPressed( sf::Mouse::Left ) ) {
        
        if ( !p_selected_object && !clicked && !selection_stop ){
            clicked = true;
            mouseOnClickStart = mousePosf;
        }

        if ( clicked ){
            mouseDrawnBox.setFillColor( sf::Color( 0, 200, 0, 80 ) );
            Window->draw( mouseDrawnBox );
        }

    }

}



void Engine::Update( const float* delta_time ){
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

float Engine::getFramesPerSecond( ){
    return 0.0;
}

void Engine::UI( ) {
    sf::Text spawn_size_text;
    spawn_size_text.setFont(font);
    spawn_size_text.setString("Spawn Size: " +  std::to_string( (int) spawn_size ) );
    spawn_size_text.setCharacterSize(35);

    sf::Text mode;
    mode.setFont(font);
    mode.setString((select_mode) ? "Multi Select Mode" : "Single Select Mode");
    mode.setCharacterSize(35);
    mode.setPosition(0,30);

    if ( select_mode && selection_stop ){
        sf::Text selection;
        selection.setFont(font);
        selection.setString((selection_stop) ? "Selection disabled: Press Escape To Enable": "");
        selection.setCharacterSize(35);
        selection.setPosition(0,mode.getPosition().y+30);
        Window->draw(selection);

    }

    Window->draw(mode);

    Window->draw(spawn_size_text);
}

void Engine::Render( ){
    if ( p_selected_object != nullptr && sf::Mouse::isButtonPressed( sf::Mouse::Right ) )
    {
        std::pair<float, float> position( p_selected_object->getPosition() );
        float distance = calculateDistance( position, std::pair<float, float> ( mousePosf.x, mousePosf.y ) );
        if ( distance > p_selected_object->getMass()*1.15f )
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

    if ( p_selected_object && sf::Mouse::isButtonPressed( sf::Mouse::Left )){
        sf::Shape* sh = p_selected_object->getShape( );
        sh->setOutlineColor( sf::Color::Red );
        Window->draw( *sh );

    }

    if ( focus ){
        sf::Vector2i currentCoordinates = Window->mapCoordsToPixel( sf::Vector2f{( (float) ( Window->getSize().x )/2 ), ( (float) ( Window->getSize().y ) / 2 )} );
        sf::Vector2i offset = currentCoordinates - originalCoordinates;
        
        mainView.move( offset.x, offset.y );

        Window->setView( mainView );

        if (offset.x == 0 && offset.y == 0) {
            focus = false;
        }
    }

    DragRectangle( );

}

void Engine::zoomViewAt( sf::Vector2i pixel, float zoom )
{
	const sf::Vector2f beforeCoord{ Window->mapPixelToCoords( pixel ) };
	mainView.zoom( zoom );
	Window->setView( mainView );
	const sf::Vector2f afterCoord{ Window->mapPixelToCoords( pixel ) };
	const sf::Vector2f offsetCoords{ beforeCoord - afterCoord };
	mainView.move( offsetCoords );
	Window->setView( mainView );
}