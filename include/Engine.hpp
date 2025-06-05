#pragma once
#include <vector> // vector
#include <memory> // shared ptr, unique ptr, weak ptr
#include <utility> // 
#include <iostream> // cout
#include <cmath>
#include <stdlib.h>
#include <assert.h>
#include <time.h>
#include <condition_variable>
#include <stack>

#include "Vector2.hpp"
#include "Object.hpp"
#include "AbstractBox.hpp"
#include "Rectangle.hpp"
#include "PhysicsMath.hpp"
#include "Physics.hpp"
#include "Quadtree.hpp"
#include "UserInterface.hpp"

#include <SFML/Graphics.hpp>
#include <SFML/Window/Clipboard.hpp>

using namespace syxd;

#define DEBUG 1
#if DEBUG == 1
  #define DEBUG_PRINT(format, ...) printf(format, ##__VA_ARGS__)
#else
  #define DEBUG_PRINT(format, ...) // Do nothing //
#endif

#define shptr_obj shared_ptr<syxd::Object>
#define shptr_rect shared_ptr<syxd::Rectangle>
#define shptr_circ shared_ptr<syxd::Circle>

using Vec2 = syxd::Vector2<float>;
using std::cout;
using std::endl; 
using std::string;
using std::shared_ptr; 
using std::unique_ptr; 
using std::weak_ptr;
using std::vector;
using std::dynamic_pointer_cast;

struct WINDOW_SETTINGS {
  float MAX_FRAME_RATE; // max frame rate of this window
  uint16_t DEFAULT_WINDOW_SIZE_X; // 
  uint16_t DEFAULT_WINDOW_SIZE_Y;
  uint16_t WORLD_SIZE; // always square
  std::string WINDOW_NAME;
};

struct UI_SETTINGS {
  uint16_t h1_size; // 50
  uint16_t h2_size; // 35
  uint16_t h3_size; // 25
  uint16_t p1_size; // 20
};

class Engine {
private:
  const float CREATION_INTERVAL = 0.1f;
  const float TOGGLE_INTERVAL = 0.3f;
  const float INTERRUPT_INTERVAL = 0.01f; 
  const float COMMAND_MODE_INTERVAL = 0.2f; 

  const float ZOOM_AMOUNT = 1.1f;
  
  float _MOVE_SENSITIVITY = 1.0f;
  float _INPUT_INTERVAL = 0.1f;
  float _CURSOR_BLINK_INTERVAL = 0.50f;

  float MAX_SIM_SPEED = 8.0f;

  const std::string FONT_PATH = "static/fonts/";
  const std::string fonts[5] =  
    { 
      FONT_PATH + "cairo.ttf",
      FONT_PATH + "FiraCode.ttf",
      FONT_PATH + "Silver.ttf",
      FONT_PATH + "simsun.ttf"
    };
  
  std::chrono::high_resolution_clock::time_point start;
  sf::Clock clock; // to keep track of time between frames
  sf::View m_main_view; // main view of the world
  sf::View m_ui_view; // ui has a seperate view so that zooming/moving through the world doesn't affect the ui
  sf::View m_help_view;
  sf::Font m_default_font;
  sf::Font m_input_font;

   
  sf::Vector2i m_mouse_pos_i; // latest mouse position as int
  sf::Vector2f m_mouse_pos_f; // latest mouse position as float
    
  float m_elapsed_time_spawn = CREATION_INTERVAL;
  float m_elapsed_time_move = INTERRUPT_INTERVAL;
  float m_elapsed_time_input = _INPUT_INTERVAL;
  float m_elapsed_time_cursor_blink = _CURSOR_BLINK_INTERVAL;
  float m_elapsed_diagnostic = TOGGLE_INTERVAL;
  float m_elapsed_time_increase = CREATION_INTERVAL;

  bool m_select_mode = false; // multi select mode
  bool m_command_mode = false; // input/command mode
  const float m_default_drag = -0.7f;
  bool m_gizmos_mode = false;
  bool m_gravity_mode = true;
  float m_drag;
  bool is_running;
  int num_objects_to_spawn = 1;
  float sim_speed = 1;
  bool is_paused = false;
  sf::Color BACKGROUND_COLOR = sf::Color::Black;
  sf::Color TEXT_COLOR = sf::Color::White;




public:
  shared_ptr<sf::RenderWindow> WINDOW; // Main render window that displays stuff
  std::shared_ptr<sf::RenderWindow> HELP_WINDOW; // Help window

  std::unique_ptr<Quadtree> m_quad_root; // Root of the quadtree of this world
  
  WINDOW_SETTINGS m_window_settings; // window/world settings
  UI_SETTINGS m_ui_settings; // ui settings

  UserInterface m_user_interface; // this engine's user interface
  UserInterface m_help_interface; // help user interface

  sf::Event e_event;

  vector<shptr_obj> p_objects; // all objects
  vector<shptr_obj> p_selected_objects; // selected objects
  shptr_obj p_selected_object = nullptr;

  Engine( const uint16_t world_size );
  ~Engine();

  void EventManager( const float& delta_time ); // event and input manager
  void CollisionCheck( ); // checks if any collision has occured and provides a response to that collision 
  void UpdatePhysics( const float& delta_time ); // updates object data
  void Render( ); // render any non-ui and non-world elements
  void Update_UI( ); // Updates UI every frame
  
  void MainLoop(); // main engine loop
  
  void addObject( const shptr_obj object ); // adds a new object to the world
  vector<shptr_obj>& getAllObjects(); // returns all objects in the world
  void zoomViewAt( const sf::Vector2i& pixel, const float& zoom ); // zoom
  void getObjectsInArea( const AbstractBox<float>& rect_size ); // returns all objects in a selected area
  void dragRectangle( ); // draws a rectangle to select objects in an area
  void objectDefault( ); // returns objects its default configuration
  void moveSelection( const sf::Vector2f& delta ); // moves selected objects
  void checkObjectsSelected( ); // check if an object is selected from a specified area
  void moveAll( vector<shptr_obj> objects, const sf::Vector2f delta ); // move all objects that are selected
  bool deleteObject( const shptr_obj object_to_delete, vector<shared_ptr<Object>>& all_objects ); // delete a selected object
  void deleteSelectedObjects( vector<shptr_obj>& objects_to_delete ); // deleted all objects that are selected
  void InitializeWorld( ); // initialize the world with objects
  void displayDiagnosticInfo( const std::chrono::high_resolution_clock::time_point& start ); // returns window frames per second
  void InitializeUI( ); // initialize the ui with elements
  void setZoomLimits( const sf::Vector2f& worldSize, const sf::Vector2f& windowSize ); // zoom limits of the world
  
  void displayGizmos( );
  
  void checkCollisionWithWorld( const shptr_obj object ) const noexcept;
  void drawEdges( );
  
  bool isRunning(); // is this engine running?
  bool isGizmosMode();
  void setGizmosMode( const bool& b );
  bool isCommandMode();
  void setCommandMode( const bool& b );
  bool isSelectMode();
  void setSelectMode( const bool& b );
  float getDrag();
  void setDrag( const float& drag );
  const float getDefaultDrag();
  bool isGravityMode();
  void setGravityMode( const bool& b );
  void setSimulationSpeed( const float& f );
  void createHelpWindow( const WINDOW_SETTINGS& window_settings );
  sf::Vector2f& getMousePosf();
  sf::Vector2i& getMousePosi();
  
  sf::View& getMainView();
};
