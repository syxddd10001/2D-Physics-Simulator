#include <iostream>
#include <vector>
#include <utility>
#include <thread>
#include <iostream>
#include <SFML/Graphics.hpp>
#include <Circle.hpp>
#include <PhysicsMath.hpp>
#include <Engine.hpp>


int main(){
    Engine* Instance = new Engine(  );

    while ( Instance->Window->isOpen() ) {
        Instance->Window->clear( );   

        Instance->mousePosf = Instance->Window->mapPixelToCoords(sf::Mouse::getPosition(*(Instance->Window)));
        Instance->mousePos = { static_cast<int>( Instance->mousePosf.x ), static_cast<int>( Instance-> mousePosf.y ) };   
        
        
        float delta_time = Instance->clock.restart().asSeconds();
        Instance->elapsed_time_spawn += delta_time;
        Instance->elapsed_time_move += delta_time; 

        Instance->EventManager( );
        
        Instance->collisionCheck( );

        Instance->Update( &delta_time );

        Instance->Render( );

        Instance->Window->setView( Instance->UIView );
        Instance->UI( );
        Instance->Window->setView( Instance->mainView );
        

        Instance->Window->display( );

    }
    return EXIT_SUCCESS;
}