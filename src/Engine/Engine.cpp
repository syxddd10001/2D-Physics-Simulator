#include <Engine.hpp>
#include <Command.hpp>
#include <Quadtree.hpp>


#define EXPERIMENTAL_1 1
#define DEBUG 0
#if DEBUG == 1
#define DEBUG_PRINT(format, ...) printf(format, ##__VA_ARGS__)
#else
#define DEBUG_PRINT(format, ...) // Do nothing //
#endif

#define print cout


sf::Event e_event;
vector<shared_ptr_obj> p_objects;
vector<shared_ptr_obj> p_selected_objects;
shared_ptr_obj p_selected_object = nullptr;

sf::Vector2f mouse_pos_prev;
sf::Vector2f mouse_pos_prev_all;

sf::Vector2f half_size;

string spawn_type;
float spawn_size = 5.0f;
float breakpoint = 500.f;
volatile uint32_t object_count = 0;


sf::Vector2i original_mouse_position;
sf::Vector2f mouse_on_click_start;
sf::RectangleShape mouse_drawn_box; // for object selection

bool clicked = false;
bool dragging = false;
bool mouseonobj = false;
bool selection_lock = false;
bool objects_selected = false;
bool deleted = false;
bool input_lock = true;
bool cursor_show = true;
bool focus = false;

sf::Vector2i original_coordinates; // where the camera originally is

// command mode blink cursor position
volatile int32_t cursor_position = 0;
// store command mode input text 
string s_input_text = "";
// command mode text ui
sf::Text inputBox;
// physical cursor for command mode
sf::RectangleShape cursor;
sf::Text command_indicator;

string input_previous = "";

// Singleton Recevier that receives and executes commands
shared_ptr<Receiver> receiver;

AbstractBox<float> box;
AbstractBox<float> mouse_query_box;
vector<shared_ptr<Object>> potential_selection;

Engine::Engine( ) { 
  m_window_settings = { 144.0f, 1000, 1000, "2D Physics Simulator" };
  m_ui_settings = { 50, 35, 25, 20 }; // h1, h2, h3, p font sizes
  
  WINDOW = std::make_shared<sf::RenderWindow>( sf::VideoMode( m_window_settings.DEFAULT_WINDOW_SIZE_X, 
                                                              m_window_settings.DEFAULT_WINDOW_SIZE_Y ),
                                                              m_window_settings.WINDOW_NAME );
  
  m_main_view = sf::View( sf::FloatRect( 0, 0, WINDOW->getSize().x, WINDOW->getSize().y ) );
  half_size = sf::Vector2f( WINDOW->getSize().x/2, WINDOW->getSize().y/2 ); // half length of the screen
  receiver = Receiver::GetInstance(); // receives and executes commands
  original_coordinates = WINDOW->mapCoordsToPixel(sf::Vector2f{ ((float)(half_size.x)), ((float)(half_size.y))}); 
  
  //box for quadtree
  box = AbstractBox<float>( Vec2( -half_size.x, -half_size.y ), Vec2( WINDOW->getSize() ) );
  //m_root mode of the quadtree
  m_root = std::make_unique<Quadtree>(box, 4, 8);
  
  m_drag = m_default_drag;
  spawn_type = "cir";
  m_gizmos_mode = true;
  
  p_selected_objects.reserve(50);
  
  #if DEBUG
    if ( !m_default_font.loadFromFile( "static/fonts/Silver.ttf" ) ){
        print << "Silver.tff font not found\n";
    }
  #else
    if ( !m_default_font.loadFromFile( "static/fonts/Silver.ttf" ) ){
        print << "Silver.tff font not found\n";
    }
  #endif

  // For the UI
  inputBox = sf::Text( "Input text: ", m_default_font ); // the input box
  inputBox.setCharacterSize( m_ui_settings.h2_size );
  cursor.setSize( sf::Vector2f { 5.0f, 20.0f } ); // cursor for text input
  cursor.setFillColor( sf::Color::White );
  command_indicator.setFont( m_default_font ); // the '>' to indicate commands
  command_indicator.setCharacterSize( m_ui_settings.h2_size );
  command_indicator.setString("> "); 
  
  sf::Vector2f move_amount { -half_size.x, -half_size.y } ; // move camera s.t the screen center is 0,0
  m_main_view.move( move_amount );
  
  InitializeSetup();
  
  WINDOW->setFramerateLimit( m_window_settings.MAX_FRAME_RATE );
  WINDOW->setView( m_main_view );
  print << std::setprecision(13);

  DEBUG_PRINT( "%dx%d window spawned \n", m_window_settings.DEFAULT_WINDOW_SIZE_X, m_window_settings.DEFAULT_WINDOW_SIZE_Y );
  
}

Engine::~Engine( ) {
  
}
/*
 Events and input manager

 Checks for input events and makes an appropriate response
*/
void Engine::EventManager( ) {
  if( WINDOW->pollEvent( e_event ) ) {
    switch( e_event.type ){
      case sf::Event::Closed:
          WINDOW->close();
      break;
      
      case sf::Event::Resized:
          WINDOW->setView( m_main_view = sf::View( sf::FloatRect( 0.0f, 0.0f, e_event.size.width, e_event.size.height ) ) );
          WINDOW->setView( m_ui_view = sf::View( sf::FloatRect( 0.0f, 0.0f, e_event.size.width, e_event.size.height ) ) );
      break;
       
      case sf::Event::TextEntered:
          
        if ( !input_lock && m_command_mode ) {
          if ( std::isprint(e_event.text.unicode) ){
            char ch = static_cast<char>(e_event.text.unicode);
            s_input_text.insert(cursor_position, 1, ch);
            cursor_position++;
          }
        }
  
      break;
        
      case sf::Event::MouseButtonPressed:
        if ( e_event.mouseButton.button == sf::Mouse::Middle ) dragging = true;
  
        if ( e_event.mouseButton.button ==  sf::Mouse::Right ) {
            
          if ( !p_selected_object && !m_select_mode ) { 

            for ( auto& selected : potential_selection ) {
              if ( selected->mouseOnObject( Vec2(m_mouse_pos_f) ) ) 
              {
                  p_selected_object = selected;                             
                  break;
              }
            }
          }
        }
  
        if ( e_event.mouseButton.button == sf::Mouse::Left && !m_select_mode ) {
            
          if ( !p_selected_object ) {

            for ( const auto& selected : potential_selection )
            {
              print << selected->getID() << '\n';
              if ( selected->mouseOnObject( Vec2(m_mouse_pos_f)) )
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
        if ( e_event.mouseButton.button == sf::Mouse::Right ) {
          if ( p_selected_object != nullptr )
          {     
            float launch_speed = 3.5f;
            Vec2 position ( p_selected_object->getPosition() ); 
            float velocity_x = launch_speed * ( position.x - m_mouse_pos_f.x );
            float velocity_y = launch_speed * ( position.y - m_mouse_pos_f.y );
            p_selected_object->setVelocity( Vec2( velocity_x, velocity_y ) );
            
            shared_ptr<sf::Shape> sh = p_selected_object->getShape();
            sh->setOutlineColor(sf::Color::Black);
            sh->setOutlineThickness( 0.5f );
            
  
            p_selected_object = nullptr;
          }
  
          if ( m_select_mode ) {
            clicked = false;
            mouse_drawn_box.setSize( sf::Vector2f( { 0.0f, 0.0f } ) );
          }
  
          if ( m_select_mode && objects_selected ) {
            objects_selected = false;
          }
          
        }
  
        if ( e_event.mouseButton.button == sf::Mouse::Left ) {     
          if ( p_selected_object && !deleted){
            shared_ptr<sf::Shape> sh = p_selected_object->getShape();
            sh->setOutlineColor( sf::Color::Black );
    
            p_selected_object = nullptr;
          }
        }
    
        if ( e_event.mouseButton.button == sf::Mouse::Middle ) {
          dragging = false;
        }
  
      break;
  
      case sf::Event::KeyReleased:
        if ( e_event.key.code == sf::Keyboard::G ) {
          m_gizmos_mode = !m_gizmos_mode;
          print << "gizmos toggled" << std::endl; 
        }
        
        if ( input_lock && m_command_mode && e_event.key.code == sf::Keyboard::P ) {
          s_input_text = input_previous;
        } 
        
        if ( e_event.key.code == sf::Keyboard::Enter && m_command_mode ) {
          receiver->Receive( s_input_text, this );

          if ( !s_input_text.empty() && cursor_position > 0) {
            input_previous = s_input_text;
            s_input_text.clear();
            cursor_position = 0;
          }
          if (p_objects.size() > 0) 
            print << "object id: " << p_objects[p_objects.size()-1]->getID() << 
            " mass: " << p_objects[p_objects.size()-1]->getMass() << 
            " position: " << p_objects[p_objects.size()-1]->getPosition().x << ", " 
            << p_objects[p_objects.size()-1]->getPosition().y << "\n";
        }
  
        if ( e_event.key.code == sf::Keyboard::LControl && m_command_mode ) {
          DEBUG_PRINT("%s\n", "released");
          input_lock = false;
        }
          
        if ( e_event.key.code == sf::Keyboard::F && m_elapsed_time_spawn >= INTERRUPT_INTERVAL && !m_command_mode ) {
          m_elapsed_time_spawn = 0.0f;  
          focus = true;
        }
        
        if ( e_event.key.code == sf::Keyboard::Tab && m_elapsed_time_spawn >= INTERRUPT_INTERVAL ) {
          m_elapsed_time_spawn = 0.0f;
          spawn_type = ( spawn_type == "cir" ) ? "rec" : "cir";
          print << spawn_type << '\n';
        }
  
        if ( e_event.key.code == sf::Keyboard::T && m_elapsed_time_spawn >= CREATION_INTERVAL && !m_command_mode ) {
          m_elapsed_time_spawn = 0.0f;   
          spawn_size += 10.0f;
          print << spawn_size << '\n';
        }
        if ( e_event.key.code == sf::Keyboard::Y && m_elapsed_time_spawn >= CREATION_INTERVAL && !m_command_mode ) {
          m_elapsed_time_spawn = 0.0f;   
          spawn_size -= 10.0f;
          if ( spawn_size <= 0) spawn_size = 1;
          print << spawn_size << '\n';
        }
  
        if ( e_event.key.code == sf::Keyboard::S && !m_command_mode ) {
          m_select_mode = !m_select_mode;
          objectDefault( );
        }
        if ( e_event.key.code == sf::Keyboard::Escape ) {
          if (m_command_mode) {
            if ( !s_input_text.empty() && cursor_position > 0) {    
              s_input_text.clear();
              cursor_position = 0;
            }
          }
          objectDefault( );
        }
  
        if ( e_event.key.code == sf::Keyboard::Delete && p_selected_object != nullptr ) {   
          deleteObject( p_selected_object, p_objects );
          p_selected_object = nullptr;
          deleted = false;
          (object_count > 0) ? object_count -- : object_count = 0;
         
        }
        
        if ( e_event.key.code == sf::Keyboard::Delete && p_selected_objects.size()>0 ) {   
          deleteSelectedObjects( p_selected_objects );
          p_selected_objects.clear();
          p_selected_objects.shrink_to_fit();
          deleted = false;
        }
  
      break;
  
      case sf::Event::KeyPressed:
        if ( e_event.key.control && e_event.key.code == sf::Keyboard::V && m_command_mode ){
          s_input_text = sf::Clipboard::getString();
          cursor_position = s_input_text.length();
        }
        
        if ( e_event.key.control && e_event.key.code == sf::Keyboard::C && m_command_mode ){
           sf::Clipboard::setString( s_input_text );
        }
        
        if ( e_event.key.code == sf::Keyboard::Left && 
        cursor_position > 0 && 
        m_command_mode && 
        m_elapsed_time_input >= _INPUT_INTERVAL ) {
          cursor_position --;
        }

        if ( e_event.key.code == sf::Keyboard::Right &&
        cursor_position < s_input_text.size() &&
        m_command_mode &&
        m_elapsed_time_input >= _INPUT_INTERVAL) {
          cursor_position ++;  
        }                
        
        if ( e_event.key.code == sf::Keyboard::LControl && m_command_mode ) {
          input_lock = true;
        }
        
        if ( e_event.key.code == sf::Keyboard::BackSpace && m_command_mode ) {
          if ( !s_input_text.empty() && cursor_position > 0) {    
            s_input_text.erase(cursor_position-1,1);
            cursor_position--;
          }
        }

        if ( input_lock && e_event.key.code == sf::Keyboard::BackSpace && m_command_mode ) {
          if ( !s_input_text.empty() && cursor_position > 0) {    
            s_input_text.clear();
            cursor_position = 0;
          }
        }
        
        if ( e_event.key.code == sf::Keyboard::Space && !m_command_mode && m_elapsed_time_spawn >= CREATION_INTERVAL ) {
          m_elapsed_time_spawn = 0.0f;   
          shared_ptr_obj obj;
          
          if ( spawn_type == "cir" ) {
            float t_mass = spawn_size*10.0f;
            obj = std::make_shared<Circle>( spawn_size, t_mass, m_mouse_pos_f.x, m_mouse_pos_f.y );
            obj->setID( p_objects.size() );
            print << obj << '\n'; 
          }
          else if (spawn_type == "rec") {
            float t_mass = spawn_size*10.0f;
            obj = std::make_shared<Rectangle>( t_mass, m_mouse_pos_f.x,  m_mouse_pos_f.y, spawn_size, spawn_size );
            obj->setID( p_objects.size() );
            print << obj << '\n';
          }
          if ( obj != nullptr ) {
            addObject( obj );
            m_root->insert( obj );
          };
        }
  
      break;
  
      case sf::Event::MouseWheelScrolled:
				if ( e_event.mouseWheelScroll.delta > 0 )
					zoomViewAt({ e_event.mouseWheelScroll.x, e_event.mouseWheelScroll.y }, (1.f / ZOOM_AMOUNT));
				else if ( e_event.mouseWheelScroll.delta < 0 )
					zoomViewAt({ e_event.mouseWheelScroll.x, e_event.mouseWheelScroll.y }, ZOOM_AMOUNT);
      
      break;

      case sf::Event::MouseMoved:
        const sf::Vector2i mouse_position {
            e_event.mouseMove.x, e_event.mouseMove.y
        };

        if (dragging) {
          const auto delta = WINDOW->mapPixelToCoords(mouse_position) - WINDOW->mapPixelToCoords(original_mouse_position);
          m_main_view.move(-delta);
          WINDOW->setView(m_main_view);
        }

        original_mouse_position = mouse_position;
      break;
     }
    }
    if ( !p_selected_object ) {
      mouse_pos_prev = WINDOW->mapPixelToCoords( sf::Mouse::getPosition( *WINDOW )  );    
    }
    
    if ( sf::Keyboard::isKeyPressed(sf::Keyboard::LControl ) && 
    sf::Keyboard::isKeyPressed( sf::Keyboard::I ) && 
    m_elapsed_time_input >= CREATION_INTERVAL ) {
      m_elapsed_time_input = 0.0f;
      m_command_mode = !m_command_mode;
      input_lock = false;
      DEBUG_PRINT("%s", m_command_mode ? "input mode\n" : "not input mode\n" );           
    }

    if ( sf::Event::MouseMoved && 
    sf::Mouse::isButtonPressed( sf::Mouse::Left ) &&
    p_selected_object ) {
      Vec2 delta ( m_mouse_pos_f.x-mouse_pos_prev.x, m_mouse_pos_f.y-mouse_pos_prev.y );
      Vec2 curr_pos { p_selected_object->getPosition().x+delta.x, p_selected_object->getPosition().y+delta.y };
      p_selected_object->setPosition( curr_pos );
      p_selected_object->setVelocity( Vec2 ( 0.0f, 0.0f ) );
      mouse_pos_prev = m_mouse_pos_f;
    }
  
    sf::Vector2f delta = m_mouse_pos_f - mouse_pos_prev_all;
    moveSelection( delta );
    mouse_pos_prev_all = m_mouse_pos_f; 
}
/*
Adds a new object to the world
*/
vector<shared_ptr_obj>& Engine::getAllObjects() {
  return p_objects;
}
/*
Deletes an object from the world
*/
bool Engine::deleteObject( shared_ptr_obj object_to_delete, vector<shared_ptr_obj>& all_objects ) {
  deleted = false;
  auto it = std::find( all_objects.begin( ), all_objects.end( ), object_to_delete );
  if ( it != all_objects.end( ) ){
    all_objects.erase( it );
    deleted = true;
  }
  return deleted;
}

#if EXPERIMENTAL_1
/*
Deletes multiple p_objects from the world
*/
void Engine::deleteSelectedObjects( vector<shared_ptr_obj>& objects_to_delete ) {
  for ( auto obj : objects_to_delete ) {
    if (deleteObject( obj, p_objects )) (object_count > 0) ? object_count -- : object_count = 0 ;
  }
  objects_to_delete.clear();
  deleted = true;
}
#endif
/*
Moves a selected object
*/
void Engine::moveSelection( const sf::Vector2f delta ) {
  if ( sf::Event::MouseMoved &&
      sf::Mouse::isButtonPressed( sf::Mouse::Left ) &&
      m_select_mode &&
      !deleted ) {  
    checkObjectsSelected( );
    if ( objects_selected ){
      moveAll( p_selected_objects, delta );
    }
  }
}
/*
Checks if any object from an area is selected
*/
void Engine::checkObjectsSelected( ) {
  for ( const auto& selected : p_selected_objects ) {
    mouseonobj = false;
    if ( selected->mouseOnObject( Vec2(m_mouse_pos_f) ) ) {
      mouseonobj = true;
      objects_selected = true;
      break;
    }
  }

}
/*
Moves all p_objects in a selected area
*/
void Engine::moveAll( vector<shared_ptr_obj> p_objects, const sf::Vector2f delta ) { 
  for ( auto obj : p_objects ) {  
    assert(obj != nullptr);
    
    obj->setVelocity( Vec2 ( 0.0f, 0.0f ) );
    obj->setPosition( Vec2 { obj->getPosition().x + ((delta.x * _MOVE_SENSITIVITY * 0.2f)), 
                             obj->getPosition().y + ((delta.y * _MOVE_SENSITIVITY * 0.2f)) } ) ;
                      // the 0.2f in delta * MOVE_SENSITIVITY * 0.2f is arbitrary
  }

}
/*
Turns selected p_objects into their default configuration
*/
void Engine::objectDefault( ) {
  for ( auto obj : p_selected_objects ) {
    obj->getShape()->setOutlineColor(sf::Color::Black);
    obj->getShape()->setOutlineThickness( 0.5f );
    
  }
  selection_lock = false;
  p_selected_objects.clear( );
  p_selected_objects.shrink_to_fit( );
  p_selected_objects.reserve(50);
}

/*
Gets all p_objects in a selected area
*/
void Engine::getObjectsInArea( const AbstractBox<float>& rect_size ) {
  for ( auto& obj : p_objects ) {
    if ( rect_size.contains( obj->getCenter(), obj->getSize() )) {
      p_selected_objects.push_back( obj );
      obj->getShape()->setOutlineColor( sf::Color::Red );
    }
  }
  p_selected_objects.shrink_to_fit();
}

/*
Draws a temporary rectangle to select p_objects
*/
void Engine::dragRectangle( ) {
  if ( !m_select_mode ) return ;
  
  if ( sf::Event::MouseMoved &&
  sf::Mouse::isButtonPressed( sf::Mouse::Right ) &&
  clicked && !p_selected_object ) { 
    sf::Vector2f rect_size( m_mouse_pos_f.x - mouse_on_click_start.x, m_mouse_pos_f.y - mouse_on_click_start.y );
    mouse_drawn_box.setPosition( mouse_on_click_start.x, mouse_on_click_start.y );
    mouse_drawn_box.setOutlineColor( sf::Color::White );
    mouse_drawn_box.setOutlineThickness( 1.0f );
    mouse_drawn_box.setSize( rect_size );
    getObjectsInArea( AbstractBox<float> {mouse_on_click_start.x, mouse_on_click_start.y, rect_size.x,rect_size.y } );
    WINDOW->draw( mouse_drawn_box );
  }


  if ( sf::Mouse::isButtonPressed( sf::Mouse::Right ) ) {
    
    if ( !p_selected_object && !clicked && !selection_lock ){
      clicked = true;
      objectDefault();
      mouse_on_click_start = m_mouse_pos_f;
    }
  
    if ( clicked ) {
      mouse_drawn_box.setFillColor( sf::Color( 0, 200, 0, 80 ) );
      WINDOW->draw( mouse_drawn_box );
    }
  }
}


/*
Updates p_objects (position, shape and velocity) and draws it on the screen
*/
void Engine::Update( const float* delta_time ) {  
  mouse_query_box = AbstractBox<float>( Vec2(m_mouse_pos_f)-(Vec2{50.0f, 50.0f}), Vec2{50.0f, 50.0f} *2 );
  // !! bug when querying p_objects that are larger than the mouse_query_box !! //
  potential_selection = m_root->query( mouse_query_box );

  box = AbstractBox<float>( Vec2( WINDOW->mapPixelToCoords( sf::Vector2i {0, 0} ) ) , Vec2(  m_main_view.getSize() ) );
  m_root = std::make_unique<Quadtree>( box, 4, 8 );
  
  for ( size_t i = 0; i < p_objects.size(); i++ ) {
    assert(p_objects[i] != nullptr);
    verletIntegration( p_objects[i], *delta_time, Vec2{ m_drag,m_drag } );
    shared_ptr<sf::Shape> sh = p_objects[i]->getShape();
    m_root->insert( p_objects[i] );
    
    if ( m_gizmos_mode ) WINDOW->draw(*(p_objects[i]->getQueryBox().shape) );
    
    WINDOW->draw( *sh );
  }
}

/*
Add an object to the world
*/
void Engine::addObject( const shared_ptr_obj object ) {
  p_objects.push_back( object );
  object_count++;
}
/*
Checks if any collision has occured and provides a response to that collision 
*/
void Engine::CollisionCheck( ) {
  for ( auto current : p_objects ) {
    auto obj_in_range = m_root->query( current->getQueryBox() );
    
    for ( auto& other : obj_in_range ) {
      if ( current != other && 
         ( typeid( *current ) == typeid( Circle )
         && typeid( *other ) == typeid( Circle ) ) ) {
        auto current_ref = std::dynamic_pointer_cast<Circle>( current );
        auto other_ref = std::dynamic_pointer_cast<Circle>( other );
        if ( onCollision( current_ref, other_ref ) ) {
          dynamicResponse( current_ref, other_ref );
        }    
      }
      
      else if ( ( typeid( *current ) == typeid( Circle ) && typeid( *other ) == typeid( Rectangle ) ) ) {
        auto current_ref = std::dynamic_pointer_cast<Circle>( current );
        auto other_ref = std::dynamic_pointer_cast<Rectangle>( other );
        if (onCollision( current_ref, other_ref ) ) {
          dynamicResponse( current_ref, other_ref );
        }
      }
      
      else if ( ( typeid( *current ) == typeid( Rectangle ) && typeid( *other ) == typeid( Circle ) ) ) {
        auto current_ref = std::dynamic_pointer_cast<Rectangle>( current );
        auto other_ref = std::dynamic_pointer_cast<Circle>( other );
        if (onCollision( other_ref, current_ref ) ) {
          dynamicResponse( other_ref, current_ref );
        }
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
  fps_text.setFont( m_default_font );
  fps_text.setString( "FPS: " +  std::to_string( fps ) );
  fps_text.setCharacterSize( m_ui_settings.h2_size );
  fps_text.setPosition( sf::Vector2f { (float) WINDOW->getSize().x - 120.0f, (float) WINDOW->getSize().y - 60.0f } );

  WINDOW->draw( fps_text );
}

/*
User Interface elements
*/
void Engine::UI( ) {
  sf::Text spawn_size_text;
  spawn_size_text.setFont( m_default_font );
  spawn_size_text.setString( "Spawn Size: " +  std::to_string( (int) spawn_size ) );
  spawn_size_text.setCharacterSize( m_ui_settings.h2_size );
  
  sf::Text select_mode_text;
  select_mode_text.setFont( m_default_font );
  select_mode_text.setString( 
      ( m_select_mode ) ? 
      "Multi Select Mode - Right click and Drag to select multiple p_objects and Left Click an Object to move all p_objects" : 
      "Single Select Mode - Left Click Object to move and Right click and m_drag to launch object" );
  select_mode_text.setCharacterSize( m_ui_settings.h2_size );
  select_mode_text.setPosition(0,30);
  
  
  sf::Text spawn_text;
  spawn_text.setFont( m_default_font );
  spawn_text.setString("Tab to change object type, SpaceBar to spawn an Object");
  spawn_text.setCharacterSize( m_ui_settings.h2_size );
  spawn_text.setPosition( 0, select_mode_text.getPosition().y + 30 );
  WINDOW->draw( spawn_text );
  
  
  
  sf::Text command_mode_text;
  command_mode_text.setFont( m_default_font );
  command_mode_text.setString( (!m_command_mode) ? "Ctrl + I for Command Mode" : "Ctrl + I to Exit Command Mode" );
  command_mode_text.setCharacterSize( m_ui_settings.h2_size );
  command_mode_text.setPosition( 0, select_mode_text.getPosition().y + 60 );
  
  inputBox.setPosition( sf::Vector2f { 15.f, (float)WINDOW->getSize().y - 60 } );
  
  inputBox.setString( s_input_text );
  
  cursor.setPosition( sf::Vector2f { inputBox.findCharacterPos(cursor_position).x, inputBox.findCharacterPos(cursor_position).y+17.f } );
  command_indicator.setPosition( 0, (float)WINDOW->getSize().y - 60 );
  
  if ( cursor_show && m_elapsed_time_cursor_blink >= _CURSOR_BLINK_INTERVAL ) {
    cursor_show = false;
    m_elapsed_time_cursor_blink = 0.0f;
  }
  
  if ( !cursor_show && m_elapsed_time_cursor_blink >= _CURSOR_BLINK_INTERVAL ) {
    cursor_show = true;
    m_elapsed_time_cursor_blink = 0.0f;
  }
  
  if ( cursor_show && m_command_mode ) WINDOW->draw( cursor );
  
  sf::Text num_objects;
  num_objects.setFont( m_default_font );
  num_objects.setString( "Objects: " +  std::to_string( object_count ) );
  num_objects.setCharacterSize( m_ui_settings.h2_size );
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
  if ( !m_select_mode && 
      p_selected_object != nullptr && 
      sf::Mouse::isButtonPressed( sf::Mouse::Right ) ) {
    
    Vec2 position( p_selected_object->getPosition() );
    float distance = calculateDistance( position, Vec2 ( m_mouse_pos_f.x, m_mouse_pos_f.y ) );
    
    if ( distance > breakpoint+p_selected_object->getSize().x ) {
      p_selected_object = nullptr;
    }
    
    sf::VertexArray line( sf::Lines, 10 );
    
    line[0].position = sf::Vector2f( position.x, position.y );
    line[0].color = sf::Color::Green;
    line[1].position = m_mouse_pos_f;
    line[1].color = sf::Color::Green;
    WINDOW->draw( line );
  
  }
  
  if ( p_selected_object && sf::Mouse::isButtonPressed( sf::Mouse::Left )) {
    shared_ptr<sf::Shape> sh = p_selected_object->getShape( );
    sh->setOutlineColor( sf::Color::Red );
    WINDOW->draw( *sh );
  }
  
  if ( focus ) {
    sf::Vector2i current_coordinates = WINDOW->mapCoordsToPixel( sf::Vector2f{( (float) ( WINDOW->getSize().x )/2 ), ( (float) ( WINDOW->getSize().y ) / 2 )} );
    sf::Vector2i offset = current_coordinates - original_coordinates;
    m_main_view.move( offset.x, offset.y );
    WINDOW->setView( m_main_view );

    if ( offset.x == 0 && offset.y == 0 ) {
        focus = false;
    }
  }
  dragRectangle( );
  displayGizmos( );

}
/*
Zoom
*/ 
void Engine::zoomViewAt( const sf::Vector2i& pixel, const float& zoom ) {
	const sf::Vector2f before_coord { WINDOW->mapPixelToCoords( pixel ) };
	m_main_view.zoom( zoom );
	WINDOW->setView( m_main_view );
	const sf::Vector2f after_coord { WINDOW->mapPixelToCoords( pixel ) };
	const sf::Vector2f offset_coords { before_coord - after_coord };
	m_main_view.move( offset_coords );
	WINDOW->setView( m_main_view );
}

void Engine::displayGizmos( ) {
  if ( !m_gizmos_mode ) return; 
  
  m_root->drawBox( WINDOW );
  WINDOW->draw(*(mouse_query_box.shape));
}

void Engine::InitializeSetup( ) {


}