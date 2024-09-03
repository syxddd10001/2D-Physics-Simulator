#include <Engine.hpp>
#include <Command.hpp>
#include <Quadtree.hpp>
#include "DiagnosticInfo.hpp"
#define DEBUG 1
#if DEBUG == 1
  #define DEBUG_PRINT(format, ...) printf(format, ##__VA_ARGS__)
#else
  #define DEBUG_PRINT(format, ...) // Do nothing //
#endif

using namespace syxd;

// Engine Instance local variables
sf::Event e_event;

vector<shptr_obj> p_objects;
vector<shptr_obj> p_selected_objects;
shptr_obj p_selected_object = nullptr;

sf::Vector2f mouse_pos_prev;
sf::Vector2f mouse_pos_prev_all;

sf::Vector2f half_size;

string spawn_type;
float spawn_size = 5.0f;
float breakpoint = 500.f;
uint32_t object_count = 0;


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
bool show_diagnostic = true;
bool set_zoom_limit = false;
sf::Vector2i original_coordinates; // where the camera originally is

// command mode blink cursor position
int8_t cursor_position = 0;
// store command mode input text 
string s_input_text = "";
std::stack<int8_t> cursor_position_history;
// command mode text ui
sf::Text inputBox;
// physical cursor for command mode
sf::RectangleShape cursor;


string input_previous = "";
// Singleton Recevier that receives and executes commands
shared_ptr<Receiver> p_receiver;

AbstractBox<float> box;
AbstractBox<float> mouse_query_box;
vector<shared_ptr<Object>> potential_selection;
  
float m_current_zoom = 1.0f;
float m_min_zoom = 0.5f;
float m_max_zoom = 2.0f;

uint16_t current_world_size;

Engine::Engine( const uint16_t world_size ) : m_window_settings{ 144.0f, 1500, 900, world_size, "2D Physics Simulator" } { 
  m_ui_settings = { 30, 20, 15, 12 }; // h1, h2, h3, p font sizes
  current_world_size = world_size;
  WINDOW = std::make_shared<sf::RenderWindow>( sf::VideoMode( m_window_settings.DEFAULT_WINDOW_SIZE_X, 
                                                              m_window_settings.DEFAULT_WINDOW_SIZE_Y ),
                                                              m_window_settings.WINDOW_NAME );
  
  m_main_view = sf::View( sf::FloatRect( 0, 0, WINDOW->getSize().x, WINDOW->getSize().y ) );
  half_size = sf::Vector2f( WINDOW->getSize().x/2, WINDOW->getSize().y/2 ); // half length of the screen
  p_receiver = Receiver::GetInstance(); // receives and executes commands
  original_coordinates = WINDOW->mapCoordsToPixel(sf::Vector2f{ ((float)(half_size.x)), ((float)(half_size.y))}); 
  
  //box for quadtree
  //box = AbstractBox<float>( Vec2( WINDOW->mapPixelToCoords( sf::Vector2i {0, 0} ) ) , Vec2(  m_main_view.getSize() ) );
  
  box = AbstractBox<float>( Vec2( -1*world_size, -1*world_size ), Vec2( world_size*2, world_size*2 ) );
  //root mode of the quadtree
  m_quad_root = std::make_unique<Quadtree>(box, 1, 8);
  
  m_drag = m_default_drag;
  spawn_type = "cir";
  m_gizmos_mode = true;
  
  p_selected_objects.reserve(50);
  
  #if DEBUG
    if ( !m_default_font.loadFromFile( "static/fonts/cairo.ttf" ) ){
        DEBUG_PRINT("Font not found\n");
    }
  #else
    if ( !m_default_font.loadFromFile( "static/fonts/cairo.ttf" ) ){
        DEBUG_PRINT("Font not found\n");
    }
  #endif

  // For the UI
  inputBox = sf::Text( "Input text: ", m_default_font ); // the input box
  inputBox.setCharacterSize( m_ui_settings.h2_size );
  cursor.setSize( sf::Vector2f { 5.0f, 20.0f } ); // cursor for text input
  cursor.setFillColor( sf::Color::White );

  m_user_interface.SetFont( m_default_font );
  
  WINDOW->setFramerateLimit( m_window_settings.MAX_FRAME_RATE );
  WINDOW->setView( m_ui_view = sf::View( sf::FloatRect( 0.0f, 0.0f, static_cast<float>(m_window_settings.DEFAULT_WINDOW_SIZE_X), 
                                                                  static_cast<float>(m_window_settings.DEFAULT_WINDOW_SIZE_Y) ) ));
  WINDOW->setView( m_main_view = sf::View( sf::FloatRect( 0.0f, 0.0f, static_cast<float>(m_window_settings.DEFAULT_WINDOW_SIZE_X), static_cast<float>(m_window_settings.DEFAULT_WINDOW_SIZE_Y) ) ) );
  setZoomLimits( sf::Vector2f { static_cast<float>(m_window_settings.WORLD_SIZE), static_cast<float>(m_window_settings.WORLD_SIZE) } , 
                 sf::Vector2f { static_cast<float>(m_window_settings.DEFAULT_WINDOW_SIZE_X), static_cast<float>(m_window_settings.DEFAULT_WINDOW_SIZE_Y) });
  sf::Vector2f move_amount { -half_size.x, -half_size.y } ; // move camera s.t the screen center is 0,0
  m_main_view.move( move_amount );

  InitializeWorld();
  InitializeUI();
  init();
  cout << std::setprecision(13);

  DEBUG_PRINT( "%dx%d window spawned \n", m_window_settings.DEFAULT_WINDOW_SIZE_X, m_window_settings.DEFAULT_WINDOW_SIZE_Y );
  is_running = true;
}

Engine::~Engine( ) {
  
}
/*
 Events and input manager

 Checks for input events and makes an appropriate response
*/
void Engine::EventManager( const float& delta_time ) {
  ( m_elapsed_time_spawn < CREATION_INTERVAL ) ? m_elapsed_time_spawn += delta_time : m_elapsed_time_spawn = CREATION_INTERVAL;
  ( m_elapsed_time_move < INTERRUPT_INTERVAL ) ? m_elapsed_time_move += delta_time : m_elapsed_time_move = INTERRUPT_INTERVAL;
  ( m_elapsed_time_input < _INPUT_INTERVAL ) ? m_elapsed_time_input += delta_time : m_elapsed_time_input = TOGGLE_INTERVAL;
  ( m_elapsed_time_cursor_blink < _CURSOR_BLINK_INTERVAL ) ? m_elapsed_time_cursor_blink += delta_time : m_elapsed_time_cursor_blink = _CURSOR_BLINK_INTERVAL;
  ( m_elapsed_diagnostic < TOGGLE_INTERVAL ) ? m_elapsed_diagnostic += delta_time : m_elapsed_diagnostic = TOGGLE_INTERVAL;
  ( m_elapsed_time_increase < CREATION_INTERVAL ) ? m_elapsed_time_increase += delta_time : m_elapsed_time_increase = CREATION_INTERVAL;
  
  if( WINDOW->pollEvent( e_event ) ) {
    switch( e_event.type ) {
      case sf::Event::Closed:
        is_running = false;
        WINDOW->close();
        
      break;

      case sf::Event::Resized:
        //setZoomLimits ( sf::Vector2f {m_window_settings.WORLD_SIZE, m_window_settings.WORLD_SIZE}, sf::Vector2f(WINDOW->getSize()));
        WINDOW->setView( m_main_view = sf::View( sf::FloatRect( 0.0f, 0.0f, e_event.size.width, e_event.size.height ) ) );
        WINDOW->setView( m_ui_view = sf::View( sf::FloatRect( 0.0f, 0.0f, e_event.size.width, e_event.size.height ) ) );
      break;

      case sf::Event::TextEntered:
          
        if ( !input_lock && m_command_mode ) {
          if ( std::isprint(e_event.text.unicode) && s_input_text.length() <= 100) {
            char ch = static_cast<char>( e_event.text.unicode );
            s_input_text.insert(cursor_position, 1, ch);
            cursor_position_history.push(cursor_position);
            cursor_position++;
          }
        }
  
      break;
        
      case sf::Event::MouseButtonPressed:
        if ( e_event.mouseButton.button == sf::Mouse::Middle ) dragging = true;
  
        if ( e_event.mouseButton.button ==  sf::Mouse::Right ) {
          if ( !p_selected_object && !m_select_mode ) { 
            for ( auto& selected : potential_selection ) {
              if ( selected->mouseOnObject( Vec2(m_mouse_pos_f) ) ) {
                p_selected_object = selected;                             
                break;
              }
            }
          }
        }
  
        if ( e_event.mouseButton.button == sf::Mouse::Left && !m_select_mode ) {  
          if ( !p_selected_object ) {
            for ( const auto& selected : potential_selection ) {
              if ( selected->mouseOnObject( Vec2(m_mouse_pos_f)) ) {
                p_selected_object = selected;
                break;
              }
            }
          } else {
              p_selected_object->getShape()->setOutlineColor( sf::Color::Red );
              p_selected_object->getShape()->setOutlineThickness( 3.0f );                                  
          }
        }       
      break;
  
      case sf::Event::MouseButtonReleased:
        if ( e_event.mouseButton.button == sf::Mouse::Right ) {
          if ( p_selected_object != nullptr ) {     
            float launch_speed = 5.f;
            Vec2 position ( p_selected_object->getPosition() ); 
            float velx = launch_speed * ( position.x - m_mouse_pos_f.x );
            float vely = launch_speed * ( position.y - m_mouse_pos_f.y );
            p_selected_object->setVelocity(Vec2( velx, vely ) );        
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
          if ( p_selected_object && !deleted) {
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
        if ( e_event.key.code == sf::Keyboard::Pause ){
          is_paused = !is_paused;
        }

        if ( input_lock && m_command_mode && e_event.key.code == sf::Keyboard::P ) {
          s_input_text = input_previous;
        }  
        if ( e_event.key.code == sf::Keyboard::Enter && m_command_mode ) {
          p_receiver->Receive( s_input_text, this );

          if ( !s_input_text.empty() && cursor_position > 0) {
            input_previous = s_input_text;
            s_input_text.clear();
            cursor_position = 0;
          }
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
        }
        
        if ( e_event.key.code == sf::Keyboard::T && m_elapsed_time_spawn >= CREATION_INTERVAL && !m_command_mode ) {
          m_elapsed_time_spawn = 0.0f;   
          spawn_size += 10.0f;
        }
        
        if ( e_event.key.code == sf::Keyboard::Y && m_elapsed_time_spawn >= CREATION_INTERVAL && !m_command_mode ) {
          m_elapsed_time_spawn = 0.0f;   
          spawn_size -= 10.0f;
          if ( spawn_size <= 0) spawn_size = 1;
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
            
            if ( s_input_text.empty() ){
              m_command_mode = false;
            }
          }
          objectDefault( );
        }
        
        if ( e_event.key.code == sf::Keyboard::Delete && p_selected_object != nullptr ) {   
          deleteObject( p_selected_object, p_objects );
          p_selected_object = nullptr;
          deleted = false;    
        }
        
        if ( e_event.key.code == sf::Keyboard::Delete && p_selected_objects.size()>0 ) {   
          deleteSelectedObjects( p_selected_objects );
          p_selected_objects.clear();
          p_selected_objects.shrink_to_fit();
          deleted = false;
        }
  
      break;
  
      case sf::Event::KeyPressed:
        if ( e_event.key.control && 
          e_event.key.code == sf::Keyboard::G &&
          m_elapsed_diagnostic >= TOGGLE_INTERVAL ) {
          m_elapsed_diagnostic = 0.0f;
          m_gizmos_mode = !m_gizmos_mode;
          DEBUG_PRINT("%s\n", "gizmos toggled"); 
        }   
        
        if ( e_event.key.control &&
          e_event.key.code == sf::Keyboard::Z && 
          m_elapsed_diagnostic >= TOGGLE_INTERVAL &&
          !cursor_position_history.empty() ) {
          m_elapsed_diagnostic = 0.0f;
          uint8_t top = cursor_position_history.top();
          s_input_text.erase(top,1);
          cursor_position_history.pop();
          cursor_position_history.empty() ? cursor_position = 0 : cursor_position = cursor_position_history.top()+1;
        }

        if ( e_event.key.control &&
          e_event.key.code == sf::Keyboard::D && 
          m_elapsed_diagnostic >= TOGGLE_INTERVAL ){
          m_elapsed_diagnostic = 0.0f;
          show_diagnostic = !show_diagnostic;
          DEBUG_PRINT("%s\n", "Diagnostic toggled"); 
        }

        if ( e_event.key.control &&
          e_event.key.code == sf::Keyboard::V &&
          m_command_mode ) {
          std::string clipboard = sf::Clipboard::getString();
          s_input_text.insert(cursor_position, clipboard);
          cursor_position += clipboard.length();
        }

        if ( e_event.key.control &&
          e_event.key.code == sf::Keyboard::A ) {
          getObjectsInArea( box );
        }

        if ( e_event.key.control &&
        e_event.key.code == sf::Keyboard::C &&
        m_command_mode ) {
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
          if ( !s_input_text.empty() && cursor_position > 0 ) {    
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
        
        if ( e_event.key.code == sf::Keyboard::Equal && 
          m_elapsed_time_increase >= CREATION_INTERVAL ){
          m_elapsed_time_increase = 0.0f;
          num_objects_to_spawn ++;
          std::cout << std::to_string(num_objects_to_spawn) << "\n";
        }
        
        if ( e_event.key.code == sf::Keyboard::Hyphen && 
          m_elapsed_time_increase >= CREATION_INTERVAL ){
          m_elapsed_time_increase = 0.0f;
          ( num_objects_to_spawn > 0 ) ? num_objects_to_spawn -- : num_objects_to_spawn;
          std::cout << std::to_string(num_objects_to_spawn) << "\n";
          
        }
        
        if ( e_event.key.code == sf::Keyboard::Space && !m_command_mode && m_elapsed_time_spawn >= CREATION_INTERVAL ) {
          m_elapsed_time_spawn = 0.0f;   
          shptr_obj obj;

          if ( spawn_type == "cir" ) {
            float t_mass = spawn_size*100.0f;
            string command = "";
            command.append("spawn circle ");
            command.append(std::to_string(t_mass) + " ");
            command.append(std::to_string(spawn_size) + " " + std::to_string(spawn_size) + " ");
            command.append(std::to_string(m_mouse_pos_f.x) + " " + std::to_string(m_mouse_pos_f.y) + " ");
            command.append(std::to_string(num_objects_to_spawn));
            p_receiver->Receive( command, this );

          } else if ( spawn_type == "rec" ) {
            float t_mass = spawn_size*100.0f;
            string command = "";
            command.append("spawn rectangle ");
            command.append(std::to_string(t_mass) + " ");
            command.append(std::to_string(spawn_size) + " " + std::to_string(spawn_size) + " ");
            command.append(std::to_string(m_mouse_pos_f.x) + " " + std::to_string(m_mouse_pos_f.y) + " ");
            command.append(std::to_string(num_objects_to_spawn));
            p_receiver->Receive( command, this );
          }
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
        if ( dragging ) {
          const auto delta = WINDOW->mapPixelToCoords(mouse_position) - WINDOW->mapPixelToCoords(original_mouse_position);
          m_main_view.move( -delta );
          WINDOW->setView( m_main_view );
        }
        original_mouse_position = mouse_position;
      break;
     }
    }
    if ( !p_selected_object ) {
      mouse_pos_prev = WINDOW->mapPixelToCoords( sf::Mouse::getPosition( *WINDOW )  );    
    }

    if ( sf::Keyboard::isKeyPressed( sf::Keyboard::LControl ) && 
    sf::Keyboard::isKeyPressed( sf::Keyboard::I ) &&
    m_elapsed_time_input >= TOGGLE_INTERVAL ) {
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
      p_selected_object->setVelocity( Vec2 {0, 0} );

      mouse_pos_prev = m_mouse_pos_f;
      
    }

    if ( !m_select_mode && !p_selected_object && sf::Mouse::isButtonPressed( sf::Mouse::Left )) {
      for ( auto& obj : p_objects ) {
        Vec2 acc = Vec2{m_mouse_pos_f} - obj->getPosition();
        float mouse_pull_strength = 100.0f;
        obj->Acceleration( acc.normalize()*mouse_pull_strength );
      }
    }

    sf::Vector2f delta = m_mouse_pos_f - mouse_pos_prev_all;
    moveSelection( delta );
    mouse_pos_prev_all = m_mouse_pos_f;
}

/*
  Add an object to the world
*/
void Engine::addObject( const shptr_obj object ) {
  p_objects.push_back( object );
  object_count++;
}

/*
  Gets all objects in the world
*/
vector<shptr_obj>& Engine::getAllObjects() {
  return p_objects;
}

/*
  Deletes an object from the world
*/
bool Engine::deleteObject( shptr_obj object_to_delete, vector<shptr_obj>& all_objects ) {
  deleted = false;
  auto it = std::find( all_objects.begin( ), all_objects.end( ), object_to_delete );
  if ( it != all_objects.end( ) ) {
    all_objects.erase( it );
    object_to_delete.reset();
    deleted = true;
  }
  deleted ? object_count -- : object_count = 0;
  return deleted;
}

/*
  Deletes multiple objects from the world
*/
void Engine::deleteSelectedObjects( vector<shptr_obj>& objects_to_delete ) {
  for ( auto obj : objects_to_delete ) {
    deleteObject( obj, p_objects );
  }
  deleted = true;
}

/*
  Moves a selected object
*/
void Engine::moveSelection( const sf::Vector2f& delta ) {
  if ( sf::Event::MouseMoved &&
      sf::Mouse::isButtonPressed( sf::Mouse::Left ) &&
      m_select_mode &&
      !deleted ) {  
    checkObjectsSelected( );
    if ( objects_selected ) {
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
  Moves all objects in a selected area
*/
void Engine::moveAll( vector<shptr_obj> objects, const sf::Vector2f delta ) { 
  for ( auto obj : objects ) {
    assert(obj != nullptr);
    obj->setVelocity( Vec2 {0, 0} );
    obj->setPosition( Vec2 { obj->getPosition().x + (delta.x * _MOVE_SENSITIVITY ),
                             obj->getPosition().y + (delta.y * _MOVE_SENSITIVITY ) } );                
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
  p_selected_objects.reserve(50);
}

/*
  Gets all p_objects in a selected area
*/
void Engine::getObjectsInArea( const AbstractBox<float>& rect_size ) {
  p_selected_objects = m_quad_root->query( rect_size );
  for ( auto& obj : p_selected_objects ) {
    obj->getShape()->setOutlineColor( sf::Color::Red );  
  }
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
    
    AbstractBox<float> range;
    
    if ( rect_size.x < 0 && rect_size.y < 0 ) 
      range = AbstractBox<float>{ Vec2 { m_mouse_pos_f.x, m_mouse_pos_f.y } , Vec2 { abs(rect_size.x), abs(rect_size.y) } };
    else if ( rect_size.x < 0 && rect_size.y > 0 ) 
      range = AbstractBox<float>{ Vec2 { m_mouse_pos_f.x, mouse_on_click_start.y }, Vec2{ abs(rect_size.x), rect_size.y }};
    else if ( rect_size.x > 0 && rect_size.y < 0)
      range = AbstractBox<float>{ Vec2 { mouse_on_click_start.x, m_mouse_pos_f.y }, Vec2{ rect_size.x, abs(rect_size.y) }};
    else 
      range = AbstractBox<float>{ Vec2 { mouse_on_click_start.x, mouse_on_click_start.y }, Vec2{ rect_size.x, rect_size.y }};
    
    getObjectsInArea( range );
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
  Updates objects (position, shape and velocity) and draws it on the screen
*/
void Engine::UpdatePhysics( const float& delta_time ) {  
  mouse_query_box = AbstractBox<float>( Vec2(m_mouse_pos_f)-(Vec2{50.0f, 50.0f}), Vec2{50.0f, 50.0f} *2 );
  // !! bug when querying objects that are larger than the mouse_query_box !! //
  potential_selection = m_quad_root->query( mouse_query_box );
  //box = AbstractBox<float>( Vec2( WINDOW->mapPixelToCoords( sf::Vector2i {0, 0} ) ) , Vec2(  m_main_view.getSize() ) );
  box = AbstractBox<float>(Vec2( -1*current_world_size, -1*current_world_size ), Vec2( current_world_size*2, current_world_size*2 ));
  m_quad_root = std::make_unique<Quadtree>( box, 1, 8 );
  // apply euler integration every frame + draw
  for ( size_t i = 0; i < p_objects.size(); i++ ) {
    assert( p_objects[i] != nullptr );
    if ( !is_paused ) p_objects[i]->EulerIntegration( delta_time );
    m_quad_root->insert( p_objects[i] );
    shared_ptr<sf::Shape> sh = p_objects[i]->getShape();
    if ( m_gizmos_mode ) WINDOW->draw(*(p_objects[i]->getQueryBox().shape) );
    WINDOW->draw( *sh );
  }
}

/*
  Checks if any collision has occured and provides a response to that collision 
*/
void Engine::CollisionCheck( ) {
  // collision detection and response
  for ( auto& current : p_objects ) {
    auto obj_in_range = m_quad_root->query( current->getQueryBox() ); 
    //checkCollisionWithWorld( current );
    if ( m_gravity_mode && !is_paused ) {
    // gravity simulation using Barnes Hut -- O(n log n)
      Vec2 force = m_quad_root->calculateForce( current ); // Use an appropriate theta value
      current->applyForce(force);
    }
    for ( auto& other : obj_in_range ) {
      if ( current == other ) continue;
      
      if ( ( typeid( *current ) == typeid( syxd::Circle )
         && typeid( *other ) == typeid( syxd::Circle ) ) ) {
        auto current_ref = dynamic_pointer_cast<syxd::Circle>( current );
        auto other_ref = dynamic_pointer_cast<syxd::Circle>( other );
        if ( onCollision( current_ref, other_ref ) ) {
          dynamicResponse( current_ref, other_ref );
        }    
      }
      else if ( ( typeid( *current ) == typeid( syxd::Circle ) && typeid( *other ) == typeid( syxd::Rectangle ) ) ) {
        auto current_ref = dynamic_pointer_cast<syxd::Circle>( current );
        auto other_ref = dynamic_pointer_cast<syxd::Rectangle>( other );
        if (onCollision( current_ref, other_ref ) ) {
          dynamicResponse( current_ref, other_ref );
        }
      }
      else if ( ( typeid( *current ) == typeid( syxd::Rectangle ) && typeid( *other ) == typeid( syxd::Circle ) ) ) {
        auto current_ref = dynamic_pointer_cast<syxd::Rectangle>( current );
        auto other_ref = dynamic_pointer_cast<syxd::Circle>( other );
        if (onCollision( other_ref, current_ref ) ) {
          dynamicResponse( other_ref, current_ref );
        }
      }
    }
  }
}

/*
  Checks collision with the world
*/
void Engine::checkCollisionWithWorld( const shptr_obj object ) const noexcept {
  Vec2 pos = object->getCenter();
  Vec2 size = object->getSize();
  const float NEG_WORLD_SIZE = -1 * current_world_size;
  
  if (pos.x - size.x <= NEG_WORLD_SIZE) {
    float overlap = NEG_WORLD_SIZE - (pos.x - size.x);
    object->setPosition(Vec2(object->getCenter().x + overlap, object->getCenter().y));
    object->setVelocity(Vec2(object->getVelocity().x*-1, object->getVelocity().y));
  }
  if (pos.y - size.y <= NEG_WORLD_SIZE) {
    float overlap = NEG_WORLD_SIZE - (pos.y - size.y);
    object->setPosition(Vec2(object->getCenter().x, object->getCenter().y + overlap));
    object->setVelocity(Vec2(object->getVelocity().x, object->getVelocity().y*-1));
  }
  if (pos.x + size.x >= current_world_size) {
    float overlap = (pos.x + size.x) - current_world_size;
    object->setPosition(Vec2(object->getCenter().x - overlap, object->getCenter().y));
    object->setVelocity(Vec2(object->getVelocity().x*-1, object->getVelocity().y)); 
  }  
  if (pos.y + size.y >= current_world_size) {
    float overlap = (pos.y + size.y) - current_world_size;
    object->setPosition(Vec2(object->getCenter().x, object->getCenter().y - overlap));
    object->setVelocity(Vec2(object->getVelocity().x, object->getVelocity().y*-1));
  }
}

/*
  Returns the Frames Per Second of the window 
*/
void Engine::displayDiagnosticInfo( const std::chrono::high_resolution_clock::time_point& start ) {
  std::chrono::high_resolution_clock::time_point end;
  float fps;
  const uint64_t cpu_usage = GetCPUUsage();
  const uint64_t memory_available = GetMemoryUsage().available;
  const uint64_t memory_used = GetMemoryUsage().used;
    // window.draw, etc.
  end = std::chrono::high_resolution_clock::now();

  fps = ((float)1e9/(float)std::chrono::duration_cast<std::chrono::nanoseconds>( end - start ).count());
  fps = round( fps * 100.0 ) / 100.0;

  float x_offset = 200.0f;
  float y_offset;

  m_user_interface.UpdateElementText(m_user_interface.FindElement("gizmos text"), (m_gizmos_mode) ? "Gizmos: ON " : "Gizmos: OFF ");
  m_user_interface.UpdateElementText(m_user_interface.FindElement("cpu text"), "CPU Used: " +  std::to_string( cpu_usage ) + "%");
  m_user_interface.UpdateElementText(m_user_interface.FindElement("memory available text"), "Available Memory: " +  std::to_string( memory_available ) + " MB");
  m_user_interface.UpdateElementText(m_user_interface.FindElement("memory used text"), "Memory Used: " +  std::to_string( memory_used ) + " MB");
  m_user_interface.UpdateElementText(m_user_interface.FindElement("fps text"), "FPS: " +  std::to_string( floor(fps)));
  m_user_interface.UpdateElementPosition(m_user_interface.FindElement("gizmos text"), sf::Vector2f { (float) WINDOW->getSize().x - x_offset, (float) WINDOW->getSize().y - 150.0f });
  m_user_interface.UpdateElementPosition(m_user_interface.FindElement("cpu text"), sf::Vector2f { (float) WINDOW->getSize().x - x_offset, (float) WINDOW->getSize().y - 125.0f });
  m_user_interface.UpdateElementPosition(m_user_interface.FindElement("memory available text"), sf::Vector2f { (float) WINDOW->getSize().x - x_offset, (float) WINDOW->getSize().y - 100.0f });
  m_user_interface.UpdateElementPosition(m_user_interface.FindElement("memory used text"), sf::Vector2f { (float) WINDOW->getSize().x - x_offset, (float) WINDOW->getSize().y - 75.0f });
  m_user_interface.UpdateElementPosition(m_user_interface.FindElement("fps text"), sf::Vector2f { (float) WINDOW->getSize().x - x_offset, (float) WINDOW->getSize().y - 50.0f });
}

/*
  User Interface elements
*/
void Engine::UI( ) {
  
  m_user_interface.UpdateElementText(m_user_interface.FindElement("spawn size"), 
                                    "Spawn Size: " + std::to_string((int)spawn_size));
  
  m_user_interface.UpdateElementText(m_user_interface.FindElement("select mode"), 
                                    ( m_select_mode ) ? 
      "Multi Select Mode - Right click and Drag to select multiple Objects and Left Click an Object to move all Objects" : 
      "Single Select Mode - Left Click Object to move and Right click and drag to launch object");
  
  m_user_interface.UpdateElementText( m_user_interface.FindElement("spawn object" ), 
                                    "Tab to change object type, SpaceBar to spawn an Object");
  
  m_user_interface.UpdateElementText( m_user_interface.FindElement("command mode"),
                                      (!m_command_mode) ? "Ctrl + I for Command Mode" : "Ctrl + I to Exit Command Mode");
  
  m_user_interface.UpdateElementText( m_user_interface.FindElement("gravity") ,
                                    ( m_gravity_mode ) ? "Gravity: ON" : "Gravity: OFF");

  
  m_user_interface.UpdateElementText( m_user_interface.FindElement("num objects" ), 
                                    "Objects: " + std::to_string(object_count));

  m_user_interface.UpdateElementPosition( m_user_interface.FindElement("command indicator"), sf::Vector2f { 0, (float)WINDOW->getSize().y - 90.0f } );
  
  inputBox.setPosition( sf::Vector2f { 15.f, (float)WINDOW->getSize().y - 90 } );
  inputBox.setString( s_input_text );
  cursor.setPosition( sf::Vector2f { inputBox.findCharacterPos(cursor_position).x, inputBox.findCharacterPos(cursor_position).y+4.f } );



  if ( cursor_show && m_elapsed_time_cursor_blink >= _CURSOR_BLINK_INTERVAL ) {
    cursor_show = false;
    m_elapsed_time_cursor_blink = 0.0f;
  }
  
  if ( !cursor_show && m_elapsed_time_cursor_blink >= _CURSOR_BLINK_INTERVAL ) {
    cursor_show = true;
    m_elapsed_time_cursor_blink = 0.0f;
  }
  
  if ( cursor_show && m_command_mode ) WINDOW->draw( cursor );
  
  WINDOW->draw( inputBox );
  m_user_interface.RenderUI( WINDOW );
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
    line[0].color = sf::Color::White;
    line[1].position = m_mouse_pos_f;
    line[1].color = sf::Color::White;
    
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
  
  if ( m_gizmos_mode ) drawEdges();
  
  dragRectangle( );
  displayGizmos( );
}

/*
  Draw edges of the world
*/
void Engine::drawEdges( ) {
  sf::VertexArray edgeLeft( sf::Lines, 10 );
  edgeLeft[0].position = sf::Vector2f( (float)(-1*current_world_size), (float)(-1*current_world_size) );
  edgeLeft[0].color = sf::Color::Green;
  edgeLeft[1].position = sf::Vector2f( (float)(-1*current_world_size), (float)(current_world_size) );
  edgeLeft[1].color = sf::Color::Red;
  
  sf::VertexArray edgeRight( sf::Lines, 10 );
  edgeRight[0].position = sf::Vector2f( (float)(current_world_size), (float)(-1*current_world_size) );
  edgeRight[0].color = sf::Color::Green;
  edgeRight[1].position = sf::Vector2f( (float)(current_world_size), (float)(current_world_size) );
  edgeRight[1].color = sf::Color::Red;
  
  sf::VertexArray edgeTop( sf::Lines, 10 );
  edgeTop[0].position = sf::Vector2f( (float)(-1*current_world_size), (float)(-1*current_world_size) );
  edgeTop[0].color = sf::Color::Green;
  edgeTop[1].position = sf::Vector2f( (float)(current_world_size), (float)(-1*current_world_size) );
  edgeTop[1].color = sf::Color::Red;
  
  sf::VertexArray edgeBottom( sf::Lines, 10 );
  edgeBottom[0].position = sf::Vector2f( (float)(-1*current_world_size), (float)(current_world_size) );
  edgeBottom[0].color = sf::Color::Green;
  edgeBottom[1].position = sf::Vector2f( (float)(current_world_size), (float)(current_world_size) );
  edgeBottom[1].color = sf::Color::Red;
  
  WINDOW->draw( edgeLeft );
  WINDOW->draw( edgeRight );
  WINDOW->draw( edgeTop );
  WINDOW->draw( edgeBottom );
}

void Engine::setZoomLimits(const sf::Vector2f& worldSize, const sf::Vector2f& windowSize) {
  m_min_zoom = std::min(windowSize.x / worldSize.x, windowSize.y / worldSize.y);
  float zoomOutFactor = 2.f;
  m_max_zoom = zoomOutFactor * std::max(worldSize.x / windowSize.x, worldSize.y / windowSize.y);
}

/*
  Zoom In/Out of View
*/ 
void Engine::zoomViewAt(const sf::Vector2i& pixel, const float& zoom) {
  float new_zoom = m_current_zoom * zoom;
  
  if ( set_zoom_limit ) { 
    if ( new_zoom < m_min_zoom ) {
        new_zoom = m_min_zoom;
    } else if ( new_zoom > m_max_zoom ) {
        new_zoom = m_max_zoom;
    }
  }
  
  const sf::Vector2f before_coord{ WINDOW->mapPixelToCoords(pixel) };
  m_main_view.zoom( new_zoom / m_current_zoom );
  m_current_zoom = new_zoom;
  const sf::Vector2f after_coord{ WINDOW->mapPixelToCoords(pixel) };
  const sf::Vector2f offset_coords{ before_coord - after_coord };
  m_main_view.move(offset_coords);
  WINDOW->setView(m_main_view);
}

void Engine::displayGizmos( ) {
  if ( !m_gizmos_mode ) return; 
  m_quad_root->drawBox( WINDOW );
  WINDOW->draw( *(mouse_query_box.shape) );
}

void Engine::InitializeWorld( ) {

}
/*
  Initialize UI elements
*/
void Engine::InitializeUI(){
  m_user_interface.InitElement( "spawn size", 
                                "Spawn Size: " +  std::to_string((int) spawn_size), 
                                m_ui_settings.h3_size, 
                                sf::Vector2f{0,0} );
  
  m_user_interface.InitElement( "select mode", ( m_select_mode ) ? 
      "Multi Select Mode - Right click and Drag to select multiple Objects and Left Click an Object to move all Objects" : 
      "Single Select Mode - Left Click Object to move and Right click and m_drag to launch object", 
      m_ui_settings.h3_size, sf::Vector2f{0,25});
  
  m_user_interface.InitElement( "spawn object", 
                                "Tab to change object type, SpaceBar to spawn an Object", 
                                m_ui_settings.h3_size, 
                                sf::Vector2f{0,50});
  
  
  m_user_interface.InitElement( "num objects",
                                "Objects: " + std::to_string(object_count),
                                m_ui_settings.h3_size,
                                sf::Vector2f{0,75});
                                
  m_user_interface.InitElement( "gravity",
                                "Gravity: " + std::to_string( m_gravity_mode ),
                                m_ui_settings.h3_size,
                                sf::Vector2f{0,100});                             
  
  m_user_interface.InitElement( "command mode", 
                              (!m_command_mode) ? "Ctrl + I for Command Mode" : "Ctrl + I to Exit Command Mode", 
                              m_ui_settings.h3_size, sf::Vector2f { 0, (float)WINDOW->getSize().y - 40 }) ;
  
  
  m_user_interface.InitElement( "command indicator",
                                "> ",
                                m_ui_settings.h2_size,
                                sf::Vector2f { 0, (float)WINDOW->getSize().y - 60 } );
                                
  m_user_interface.UpdateElementPosition(m_user_interface.FindElement("command indicator"), sf::Vector2f { 0, (float)WINDOW->getSize().y - 60 } );
                             
  float x_offset = 200.0f;
  float y_offset;

  m_user_interface.InitElement( "gizmos text", 
                              (m_gizmos_mode) ? "Gizmos: ON " : "Gizmos: OFF ",
                              m_ui_settings.h3_size,
                              sf::Vector2f { (float) WINDOW->getSize().x - x_offset, (float) WINDOW->getSize().y - 150.0f } );
  
  m_user_interface.InitElement( "cpu text", 
                              "CPU Used: " +  std::to_string( 0 ) + "%",
                              m_ui_settings.h3_size,
                              sf::Vector2f { (float) WINDOW->getSize().x - x_offset, (float) WINDOW->getSize().y - 125.0f } );
  
  m_user_interface.InitElement( "memory available text", 
                              "Available Memory: " +  std::to_string( 0 ) + " MB",
                              m_ui_settings.h3_size,
                              sf::Vector2f { (float) WINDOW->getSize().x - x_offset, (float) WINDOW->getSize().y - 100.0f } );
  
  m_user_interface.InitElement( "memory used text", 
                              "Memory Used: " +  std::to_string( 0 ) + " MB",
                              m_ui_settings.h3_size,
                              sf::Vector2f { (float) WINDOW->getSize().x - x_offset, (float) WINDOW->getSize().y - 75.0f } );


  m_user_interface.InitElement( "fps text", 
                              "FPS: " +  std::to_string( floor(0) ),
                              m_ui_settings.h3_size,
                              sf::Vector2f { (float) WINDOW->getSize().x - x_offset, (float) WINDOW->getSize().y - 50.0f } );
}

bool Engine::isRunning(){
  return is_running;
}

sf::View& Engine::getMainView(){
  return m_main_view;
}

bool Engine::isGizmosMode(){
  return m_gizmos_mode;
}
bool Engine::isCommandMode(){
  return m_command_mode;
}
bool Engine::isSelectMode(){
  return m_select_mode;
}
float Engine::getDrag(){
  return m_drag;
}
void Engine::setDrag( const float& drag ){
  m_drag = drag;
}
const float Engine::getDefaultDrag(){
  return m_default_drag;
}

void Engine::setGizmosMode( const bool& b ){
  m_gizmos_mode = b;
}

void Engine::setCommandMode( const bool& b ){
  m_command_mode = b;
}

void Engine::setSelectMode( const bool& b ){
  m_select_mode = b;
}

bool Engine::isGravityMode(){
  return m_gravity_mode;
}
void Engine::setGravityMode( const bool& b ){
  m_gravity_mode = b;
}

sf::Vector2f& Engine::getMousePosf(){
  return m_mouse_pos_f;
}
sf::Vector2i& Engine::getMousePosi(){
  return m_mouse_pos_i;
}

void Engine::MainLoop(){
  while ( isRunning() ) {
    start = std::chrono::high_resolution_clock::now(); // for benchmarking
    WINDOW->clear( );
    m_mouse_pos_f = WINDOW->mapPixelToCoords( sf::Mouse::getPosition( *(WINDOW) ) ); // current mouse pos in float
    m_mouse_pos_i = { static_cast<int>( m_mouse_pos_f.x ), static_cast<int>( m_mouse_pos_f.y ) };  // current mouse pos in int
    float delta_time = clock.restart().asSeconds(); // getting deltaTime
    EventManager( delta_time ); // calling event manager to handle inputs
    CollisionCheck( ); // calling collision checker
    UpdatePhysics( delta_time ); // updating and rerendering the positions of objects
    Render( ); // rendering any non-UI and non-world elements
    WINDOW->setView( m_ui_view ); // setting view for UI, so that UI does not change size when moving / zooming in world
    UI( ); // rendering UI 
    displayDiagnosticInfo( start );  // diagnostic info for benchmarking
    WINDOW->setView( m_main_view ); // resetting view to main 
    WINDOW->display();
  }
}