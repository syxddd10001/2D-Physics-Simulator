#include "Engine.hpp"
#include <Command.hpp>
#include <Quadtree.hpp>


#define EXPERIMENTAL_1 0
#define DEBUG 1
#if DEBUG == 1
#define DEBUG_PRINT(format, ...) printf(format, ##__VA_ARGS__)
#else
#define DEBUG_PRINT(format, ...) // Do nothing //
#endif

#define print std::cout


sf::Event evnt;
std::vector<shared_ptr_obj> objects;
std::vector<shared_ptr_obj> selected_objects;
shared_ptr_obj p_selected_object = nullptr;

sf::Vector2f mousePos_prev;
sf::Vector2f mousePos_prev_all;

sf::Vector2f HalfSize;

std::string spawn_type;
float spawn_size = 5.0f;
float breakpoint = 500.f;
volatile uint32_t object_count = 0;


sf::Vector2i original_mouse_position;
sf::Vector2f mouseOnClickStart;
sf::RectangleShape mouseDrawnBox; // for object selection

bool clicked = false;
bool dragging = false;
bool mouseonobj = false;
bool selection_lock = false;
bool objects_selected = false;
bool deleted = false;
bool input_lock = true;
bool cursor_show = true;
bool focus = false;

sf::Vector2i originalCoordinates;


// command mode blink cursor position
volatile int32_t cursor_position = 0;
// store command mode input text 
std::string input_text = "";
// command mode text ui
sf::Text inputBox;
// physical cursor for command mode
sf::RectangleShape cursor;
sf::Text command_indicator;

std::string input_previous = "";

// Singleton Recevier that receives and executes commands
std::shared_ptr<Receiver> receiver;


AbstractBox<float> box;
AbstractBox<float> mouseQueryBox;


Engine::Engine( ){ 
  window_settings = {144.0f, 1000, 1000, "2D Physics Simulator"};
  ui_settings = { 50, 35, 25, 20 }; // h1, h2, h3, p font sizes
  
  WINDOW = std::make_shared<sf::RenderWindow>( sf::VideoMode( window_settings.DEFAULT_WINDOW_SIZE_X, window_settings.DEFAULT_WINDOW_SIZE_Y ), window_settings.WINDOW_NAME );
  mainView = sf::View( sf::FloatRect( 0, 0, WINDOW->getSize().x, WINDOW->getSize().y ) );
  
  HalfSize = sf::Vector2f( WINDOW->getSize().x/2, WINDOW->getSize().y/2 ); // half length of the screen
  receiver = Receiver::GetInstance(); // receives and executes commands
  originalCoordinates = WINDOW->mapCoordsToPixel(sf::Vector2f{ ((float)(HalfSize.x)), ((float)(HalfSize.y))}); // where the camera originally is
  
  //box for quadtree
  box = AbstractBox<float>( Vec2( -HalfSize.x, -HalfSize.y ), Vec2( WINDOW->getSize() ) );
  //root mode of the quadtree
  root = std::make_unique<Quadtree>(box, 4, 8);
  
  drag = default_drag;
  spawn_type = "cir";
  gizmos_mode = true;
  
  #if DEBUG
    if ( !default_font.loadFromFile( "static/fonts/Silver.ttf" ) ){
        std::cout << "Silver.tff font not found\n";
    }
  #else
    if ( !default_font.loadFromFile( "static/fonts/Silver.ttf" ) ){
        std::cout << "Silver.tff font not found\n";
    }
  #endif

  // For the UI
  inputBox = sf::Text( "Input text: ", default_font ); // the input box
  inputBox.setCharacterSize( ui_settings.h2_size );
  cursor.setSize( sf::Vector2f { 5.0f, 20.0f } ); // cursor for text input
  cursor.setFillColor( sf::Color::White );
  command_indicator.setFont( default_font ); // the '>' to indicate commands
  command_indicator.setCharacterSize( ui_settings.h2_size );
  command_indicator.setString("> "); 
  
  sf::Vector2f move_amount { -HalfSize.x, -HalfSize.y } ; // move camera s.t the screen center is 0,0
  mainView.move( move_amount );
  
  WINDOW->setFramerateLimit( window_settings.MAX_FRAME_RATE );
  WINDOW->setView( mainView );
  print << std::setprecision(13);

  DEBUG_PRINT( "%dx%d window spawned \n", window_settings.DEFAULT_WINDOW_SIZE_X, window_settings.DEFAULT_WINDOW_SIZE_Y );
  
}

Engine::~Engine( ){
  
}
/*
 Events and input manager

 Checks for input events and makes an appropriate response
*/
void Engine::EventManager( ){
  if( WINDOW->pollEvent( evnt ) ){
    switch( evnt.type ){
      case sf::Event::Closed:
          WINDOW->close();
      break;
      
      case sf::Event::Resized:
          WINDOW->setView( mainView = sf::View( sf::FloatRect( 0.0f, 0.0f, evnt.size.width, evnt.size.height ) ) );
          WINDOW->setView( UIView = sf::View( sf::FloatRect( 0.0f, 0.0f, evnt.size.width, evnt.size.height ) ) );
      break;
       
      case sf::Event::TextEntered:
          
        if ( !input_lock && command_mode ) {
          if ( std::isprint(evnt.text.unicode) ){
            char ch = static_cast<char>(evnt.text.unicode);
            input_text.insert(cursor_position, 1, ch);
            cursor_position++;
          }
        }
  
      break;
        
      case sf::Event::MouseButtonPressed:
        if ( evnt.mouseButton.button == sf::Mouse::Middle ) dragging = true;
  
        if ( evnt.mouseButton.button ==  sf::Mouse::Right ) {
            
          if ( !p_selected_object && !select_mode ) { 
            auto potential_selection = root->query( mouseQueryBox );
            for ( auto& selected : potential_selection ) {
              if ( selected->mouseOnObject( Vec2(mousePosf) ) ) 
              {
                  p_selected_object = selected;                             
                  break;
              }
            }
          }
        }
  
        if ( evnt.mouseButton.button == sf::Mouse::Left && !select_mode ) {
            
          if ( !p_selected_object ) {
            auto potential_selection = root->query( mouseQueryBox );
            for ( auto& selected : potential_selection )
            {
              if ( selected->mouseOnObject( Vec2(mousePosf)) )
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
            Vec2 position ( p_selected_object->getPosition() ); 
            float velocity_x = launch_speed * ( position.x - mousePosf.x );
            float velocity_y = launch_speed * ( position.y - mousePosf.y );
            p_selected_object->setVelocity( Vec2( velocity_x, velocity_y ) );
            
            std::shared_ptr<sf::Shape> sh = p_selected_object->getShape();
            sh->setOutlineColor(sf::Color::Black);
  
            p_selected_object = nullptr;
          }
  
          if ( select_mode ){
            clicked = false;
            mouseDrawnBox.setSize(sf::Vector2f({0.0f,0.0f}));
          }
  
          if ( select_mode && objects_selected ) {
            objects_selected = false;
          }
          
        }
  
        if ( evnt.mouseButton.button == sf::Mouse::Left ){     
          if ( p_selected_object && !deleted){
            std::shared_ptr<sf::Shape> sh = p_selected_object->getShape();
            sh->setOutlineColor( sf::Color::Black );
    
            p_selected_object = nullptr;
          }
        }
    
        if ( evnt.mouseButton.button == sf::Mouse::Middle ) {
          dragging = false;
        }
  
      break;
  
      case sf::Event::KeyReleased:
        if ( input_lock && command_mode && evnt.key.code == sf::Keyboard::P){
          input_text = input_previous;
        } 
        
        if ( evnt.key.code == sf::Keyboard::Enter && command_mode ){
          receiver->Receive( input_text, this );

          if ( !input_text.empty() && cursor_position > 0) {
            input_previous = input_text;
            input_text.clear();
            cursor_position = 0;
          }
          if (objects.size() > 0) 
            std::cout << "object id: " << objects[objects.size()-1]->getID() << 
            " mass: " << objects[objects.size()-1]->getMass() << 
            " position: " << objects[objects.size()-1]->getPosition().x << ", " 
            << objects[objects.size()-1]->getPosition().y << "\n";
        }
  
        if ( evnt.key.code == sf::Keyboard::LControl && command_mode ){
          DEBUG_PRINT("%s\n", "released");
          input_lock = false;
        }
          
        if ( evnt.key.code == sf::Keyboard::F && elapsed_time_spawn >= INTERRUPT_INTERVAL && !command_mode ){
          elapsed_time_spawn = 0.0f;  
          focus = true;
        }
        
        if ( evnt.key.code == sf::Keyboard::Tab && elapsed_time_spawn >= INTERRUPT_INTERVAL ){
          elapsed_time_spawn = 0.0f;
          spawn_type = ( spawn_type == "cir" ) ? "rec" : "cir";
          std::cout << spawn_type << std::endl;
        }
  
        if ( evnt.key.code == sf::Keyboard::T && elapsed_time_spawn >= CREATION_INTERVAL && !command_mode ) 
        {
          elapsed_time_spawn = 0.0f;   
          spawn_size += 10.0f;
          std::cout << spawn_size << std::endl;
        }
        if ( evnt.key.code == sf::Keyboard::Y && elapsed_time_spawn >= CREATION_INTERVAL && !command_mode ) 
        {
          elapsed_time_spawn = 0.0f;   
          spawn_size -= 10.0f;
          std::cout << spawn_size << std::endl;
        }
  
        if ( evnt.key.code == sf::Keyboard::S && !command_mode ){
          select_mode = !select_mode;
          objectDefault( );
        }
        if ( evnt.key.code == sf::Keyboard::Escape ){
          if (command_mode) {
            if ( !input_text.empty() && cursor_position > 0) {    
              input_text.clear();
              cursor_position = 0;
            }
          }
          objectDefault( );
        }
  
        if ( evnt.key.code == sf::Keyboard::Delete && p_selected_object != nullptr ){   
          deleteObject( p_selected_object, objects );
          p_selected_object = nullptr;
          deleted = false;
          object_count--;
        }
  
      break;
  
      case sf::Event::KeyPressed:
        if ( evnt.key.code == sf::Keyboard::Left && cursor_position > 0 && command_mode && elapsed_time_input >= INPUT_INTERVAL){
          cursor_position --;
        }

        if ( evnt.key.code == sf::Keyboard::Right && cursor_position < input_text.size() && command_mode && elapsed_time_input >= INPUT_INTERVAL){
          cursor_position ++;  
        }                
        
        if ( evnt.key.code == sf::Keyboard::LControl && command_mode ){
          input_lock = true;
        }
        if ( evnt.key.code == sf::Keyboard::BackSpace && command_mode ){
          if ( !input_text.empty() && cursor_position > 0) {    
            input_text.erase(cursor_position-1,1);
            cursor_position--;
          }
        }

        if ( input_lock && evnt.key.code == sf::Keyboard::BackSpace && command_mode ){
          if ( !input_text.empty() && cursor_position > 0) {    
            input_text.clear();
            cursor_position = 0;
          }
        }
        
        if ( evnt.key.code == sf::Keyboard::Space && !command_mode && elapsed_time_spawn >= CREATION_INTERVAL ){
          elapsed_time_spawn = 0.0f;   
          shared_ptr_obj obj;
          
          if ( spawn_type == "cir" ) {
            float t_mass = spawn_size*10.0f;
            obj = std::make_shared<Circle>( spawn_size, t_mass, mousePosf.x, mousePosf.y );
            obj->setID( objects.size() );
            std::cout << obj << std::endl; 
          }
          else if (spawn_type == "rec") {
            float t_mass = spawn_size*10.0f;
            obj = std::make_shared<Rectangle>( t_mass, mousePosf.x,  mousePosf.y, spawn_size, spawn_size );
            obj->setID( objects.size() );
            std::cout << obj << std::endl;
          }
          if ( obj != nullptr ) {
            addObject( obj );
            root->insert( obj );
          };
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
          const auto delta = WINDOW->mapPixelToCoords(mouse_position) - WINDOW->mapPixelToCoords(original_mouse_position);
          mainView.move(-delta);
          WINDOW->setView(mainView);
        }

        original_mouse_position = mouse_position;
      break;
  
  
     }
    }
    if ( !p_selected_object ){
      mousePos_prev = WINDOW->mapPixelToCoords( sf::Mouse::getPosition( *WINDOW )  );    
    }
    
    if ( sf::Keyboard::isKeyPressed(sf::Keyboard::LControl ) && sf::Keyboard::isKeyPressed( sf::Keyboard::I ) && elapsed_time_input >= CREATION_INTERVAL ){
      elapsed_time_input = 0.0f;
      command_mode = !command_mode;
      input_lock = false;
      DEBUG_PRINT("%s", command_mode ? "input mode\n" : "not input mode\n" );           
    }
  
  
    if ( sf::Event::MouseMoved && sf::Mouse::isButtonPressed( sf::Mouse::Left ) && p_selected_object ) {
      Vec2 delta ( mousePosf.x-mousePos_prev.x, mousePosf.y-mousePos_prev.y );
      Vec2 curr_pos { p_selected_object->getPosition().x+delta.x, p_selected_object->getPosition().y+delta.y };
      p_selected_object->setPosition( curr_pos );
      p_selected_object->setVelocity( Vec2 ( 0.0f, 0.0f ) );
      mousePos_prev = mousePosf;
    }
  
    sf::Vector2f delta = mousePosf - mousePos_prev_all;
    moveSelection( delta );
    mousePos_prev_all = mousePosf;

    
}
/*
Adds a new object to the world
*/
std::vector<shared_ptr_obj>& Engine::GetAllObjects() {
  return objects;
}
/*
Deletes an object from the world
*/
void Engine::deleteObject( const shared_ptr_obj object_to_delete, std::vector<shared_ptr_obj>& all_objects ) {
  auto it = std::find( all_objects.begin( ), all_objects.end( ), object_to_delete );
  if ( it != all_objects.end( ) ){
    all_objects.erase( it );
  }
  deleted = true;
}

#if EXPERIMENTAL_1
/*
Deletes multiple objects from the world
*/
void Engine::deleteSelectedObjects( std::vector<shared_ptr_obj>& all_objects ){
  for (auto it = all_objects.begin(); it != all_objects.end(); ++it) {
    delete *it;
  }
  all_objects.clear();
}
#endif
/*
Moves a selected object
*/
void Engine::moveSelection( const sf::Vector2f delta ) {
  if ( sf::Event::MouseMoved && sf::Mouse::isButtonPressed( sf::Mouse::Left ) && select_mode && !deleted ) {  
    checkObjectsSelected( );
    if ( objects_selected ){
      moveAll( selected_objects, delta );
    }
  }
}
/*
Checks if any object from an area is selected
*/
void Engine::checkObjectsSelected( ) {
  for ( auto selected : selected_objects )
  {
    mouseonobj = false;
    if ( selected->mouseOnObject( Vec2(mousePosf) ) ) {
      mouseonobj = true;
      objects_selected = true;
      break;
    }
  }

}
/*
Moves all objects in a selected area
*/
void Engine::moveAll( std::vector<shared_ptr_obj> objects, const sf::Vector2f delta ) { 
for ( auto obj : objects ){  
  obj->setVelocity( Vec2 ( 0.0f, 0.0f ) );
  obj->setPosition( Vec2 { obj->getPosition().x + ((delta.x * MOVE_SENSITIVITY * 0.2f)), 
                           obj->getPosition().y + ((delta.y * MOVE_SENSITIVITY * 0.2f)) } ) ;
                    // the 0.2f in delta * MOVE_SENSITIVITY * 0.2f is arbitrary
}

}
/*
Turns selected objects into their default configuration
*/
void Engine::objectDefault( ) {
  for ( auto obj : selected_objects ){
    obj->getShape()->setOutlineColor(sf::Color::Black);
  }
  selection_lock = false;
  selected_objects.clear( );
  selected_objects.shrink_to_fit( );
}

/*
Gets all objects in a selected area
*/
void Engine::GetObjectsInArea( const Vec2 start, const Vec2 rect_size ) {
  for ( auto obj : objects ){
    Vec2 pos = obj->getPosition();
    if ( ( pos.x >= start.x && pos.x <= ( start.x+rect_size.x ) ) && 
         ( pos.y >= start.y && pos.y <= ( start.y + rect_size.y ) ) ){
      selected_objects.push_back( obj );
      obj->getShape()->setOutlineColor( sf::Color::Red );
    }
  }
  selected_objects.shrink_to_fit();
}

/*
Draws a temporary rectangle to select objects
*/
void Engine::DragRectangle( ) {
  if ( !select_mode ) return ;
  
  if ( sf::Event::MouseMoved && sf::Mouse::isButtonPressed( sf::Mouse::Right ) && clicked && !p_selected_object ) { 
    sf::Vector2f rect_size( mousePosf.x - mouseOnClickStart.x, mousePosf.y - mouseOnClickStart.y );
    mouseDrawnBox.setPosition( mouseOnClickStart.x, mouseOnClickStart.y );
    mouseDrawnBox.setOutlineColor( sf::Color::White );
    mouseDrawnBox.setOutlineThickness( 1.0f );
    mouseDrawnBox.setSize( rect_size );
    GetObjectsInArea( Vec2( mouseOnClickStart.x, mouseOnClickStart.y ), Vec2( rect_size.x,rect_size.y ) );
    WINDOW->draw( mouseDrawnBox );
  }


  if ( sf::Mouse::isButtonPressed( sf::Mouse::Right ) ) {
    
    if ( !p_selected_object && !clicked && !selection_lock ){
      clicked = true;
      objectDefault();
      mouseOnClickStart = mousePosf;
    }
  
    if ( clicked ){
      mouseDrawnBox.setFillColor( sf::Color( 0, 200, 0, 80 ) );
      WINDOW->draw( mouseDrawnBox );
    }
  }
}


/*
Updates objects (position, shape and velocity) and draws it on the screen
*/
void Engine::Update( const float* delta_time ) {
  mouseQueryBox = AbstractBox<float>( Vec2(mousePosf)-(Vec2{50.0f, 50.0f}), Vec2{50.0f, 50.0f} * 2 );
  //box = AbstractBox<float>( Vec2(  ) , Vec2( WINDOW->getSize() ) );
  root = std::make_unique<Quadtree>( box, 4, 8 );
  
  for ( size_t i = 0; i < objects.size(); i++ ) {
    verletIntegration( objects[i], *delta_time, Vec2{ drag,drag } );
    std::shared_ptr<sf::Shape> sh = objects[i]->getShape();
    root->insert( objects[i] );
    
    if ( gizmos_mode ) WINDOW->draw(*(objects[i]->getQueryBox().shape) );
    
    WINDOW->draw( *sh );
  }
}

/*
Add an object to the world
*/
void Engine::addObject( const shared_ptr_obj object ) {
  objects.push_back( object );
  object_count++;
}
/*
Checks if any collision has occured and provides a response to that collision 
*/
void Engine::collisionCheck( ) {
  for ( auto current : objects )
  {
    auto obj_in_range = root->query( current->getQueryBox() );
    
    for ( auto other : obj_in_range )
    {
      if ( current != other && ( typeid( *current ) == typeid( Circle ) && typeid( *other ) == typeid( Circle ) ) )
      {
        auto current_ref = std::dynamic_pointer_cast<Circle>( current );
        auto other_ref = std::dynamic_pointer_cast<Circle>( other );
        if ( onCollision( std::dynamic_pointer_cast<Circle>( current ), std::dynamic_pointer_cast<Circle>( other ) ) )
        {
          dynamicResponse( current_ref, other_ref );
        }    
      }
      
      else if ( ( typeid( *current ) == typeid( Circle ) && typeid( *other ) == typeid( Rectangle ) )){
        auto current_ref = std::dynamic_pointer_cast<Circle>( current );
        auto other_ref = std::dynamic_pointer_cast<Rectangle>( other );
        onCollision( current_ref, other_ref ); 
      }
      
      else if ( ( typeid( *current ) == typeid( Rectangle ) && typeid( *other ) == typeid( Circle ) )){
        auto current_ref = std::dynamic_pointer_cast<Rectangle>( current );
        auto other_ref = std::dynamic_pointer_cast<Circle>( other );
        onCollision( other_ref, current_ref ); 
      } 
      
    }
  }
 
}
/*
Returns the Frames Per Second of the window 
*/
void Engine::displayFramesPerSecond( const std::chrono::high_resolution_clock::time_point& start ) {
  
  std::chrono::high_resolution_clock::time_point end;
  float fps;
    // window.draw, etc.
  end = std::chrono::high_resolution_clock::now();

  fps = ((float)1e9/(float)std::chrono::duration_cast<std::chrono::nanoseconds>( end - start ).count());
  fps = round( fps * 100.0 ) / 100.0;
  sf::Text fps_text;
  fps_text.setFont( default_font );
  fps_text.setString( "FPS: " +  std::to_string( fps ) );
  fps_text.setCharacterSize( ui_settings.h2_size );
  fps_text.setPosition( sf::Vector2f { (float) WINDOW->getSize().x - 120.0f, (float) WINDOW->getSize().y - 60.0f } );

  WINDOW->draw( fps_text );
  
}

/*
User Interface elements
*/
void Engine::UI( ) {
  sf::Text spawn_size_text;
  spawn_size_text.setFont( default_font );
  spawn_size_text.setString( "Spawn Size: " +  std::to_string( (int) spawn_size ) );
  spawn_size_text.setCharacterSize( ui_settings.h2_size );
  
  sf::Text select_mode_text;
  select_mode_text.setFont( default_font );
  select_mode_text.setString( 
      ( select_mode ) ? 
      "Multi Select Mode - Right click and Drag to select multiple objects and Left Click an Object to move all objects" : 
      "Single Select Mode - Left Click Object to move and Right click and drag to launch object" );
  select_mode_text.setCharacterSize( ui_settings.h2_size );
  select_mode_text.setPosition(0,30);
  
  
  sf::Text spawn_text;
  spawn_text.setFont(default_font);
  spawn_text.setString("Tab to change object type, SpaceBar to spawn an Object");
  spawn_text.setCharacterSize( ui_settings.h2_size );
  spawn_text.setPosition( 0, select_mode_text.getPosition().y + 30 );
  WINDOW->draw( spawn_text );
  
  
  
  sf::Text command_mode_text;
  command_mode_text.setFont( default_font );
  command_mode_text.setString( (!command_mode) ? "Ctrl + I for Command Mode" : "Ctrl + I to Exit Command Mode" );
  command_mode_text.setCharacterSize( ui_settings.h2_size );
  command_mode_text.setPosition( 0, select_mode_text.getPosition().y + 60 );
  
  inputBox.setPosition( sf::Vector2f { 15.f, (float)WINDOW->getSize().y - 60 } );
  
  inputBox.setString( input_text );
  
  cursor.setPosition( sf::Vector2f { inputBox.findCharacterPos(cursor_position).x, inputBox.findCharacterPos(cursor_position).y+17.f } );
  command_indicator.setPosition( 0, (float)WINDOW->getSize().y - 60 );
  
  if ( cursor_show && elapsed_time_cursor_blink >= CURSOR_BLINK_INTERVAL ) {
    cursor_show = false;
    elapsed_time_cursor_blink = 0.0f;
  }
  
  if ( !cursor_show && elapsed_time_cursor_blink >= CURSOR_BLINK_INTERVAL ){
    cursor_show = true;
    elapsed_time_cursor_blink = 0.0f;
  }
  
  if ( cursor_show && command_mode ) WINDOW->draw( cursor );
  
  sf::Text num_objects;
  num_objects.setFont( default_font );
  num_objects.setString( "Objects: " +  std::to_string( object_count ) );
  num_objects.setCharacterSize( ui_settings.h2_size );
  num_objects.setPosition( sf::Vector2f { 0, command_mode_text.getPosition().y + 30 } );

  WINDOW->draw( command_indicator );
  WINDOW->draw( inputBox );
  WINDOW->draw( select_mode_text );
  WINDOW->draw( command_mode_text );
  WINDOW->draw( num_objects ); 
  WINDOW->draw( spawn_size_text );
}
/*
Renders non-UI and non-world elements to the screen
*/
void Engine::Render( ) {
  if ( !select_mode && 
      p_selected_object != nullptr && 
      sf::Mouse::isButtonPressed( sf::Mouse::Right ) ) {
    
    Vec2 position( p_selected_object->getPosition() );
    float distance = calculateDistance( position, Vec2 ( mousePosf.x, mousePosf.y ) );
    
    if ( distance > breakpoint ) {
      p_selected_object = nullptr;
    }
    
    sf::VertexArray line( sf::Lines, 10 );
    
    line[0].position = sf::Vector2f( position.x, position.y );
    line[0].color = sf::Color::Green;
    line[1].position = mousePosf;
    line[1].color = sf::Color::Green;
    WINDOW->draw( line );
  
  }
  
  if ( p_selected_object && sf::Mouse::isButtonPressed( sf::Mouse::Left )) {
    std::shared_ptr<sf::Shape> sh = p_selected_object->getShape( );
    sh->setOutlineColor( sf::Color::Red );
    WINDOW->draw( *sh );
  }
  
  if ( focus ) {
    sf::Vector2i currentCoordinates = WINDOW->mapCoordsToPixel( sf::Vector2f{( (float) ( WINDOW->getSize().x )/2 ), ( (float) ( WINDOW->getSize().y ) / 2 )} );
    sf::Vector2i offset = currentCoordinates - originalCoordinates;
    mainView.move( offset.x, offset.y );
    WINDOW->setView( mainView );

    if ( offset.x == 0 && offset.y == 0 ) {
        focus = false;
    }
  }
  DragRectangle( );
  displayGizmos( );

}
/*
Zoom
*/ 
void Engine::zoomViewAt( const sf::Vector2i& pixel, const float& zoom ) {
	const sf::Vector2f beforeCoord{ WINDOW->mapPixelToCoords( pixel ) };
	mainView.zoom( zoom );
	WINDOW->setView( mainView );
	const sf::Vector2f afterCoord{ WINDOW->mapPixelToCoords( pixel ) };
	const sf::Vector2f offsetCoords{ beforeCoord - afterCoord };
	mainView.move( offsetCoords );
	WINDOW->setView( mainView );
}

void Engine::displayGizmos( ){
  if (!gizmos_mode) return; 
  
  root->drawBox( WINDOW );
  WINDOW->draw(*(mouseQueryBox.shape));
}