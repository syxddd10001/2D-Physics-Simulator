#pragma once
#include <vector>
#include <memory>
#include <utility>
#include <iostream>
#include <cmath>
#include <stdlib.h>
#include <assert.h>
#include <time.h>
#include <condition_variable>
#include "Vector2.hpp"
#include "Object.hpp"
#include "AbstractBox.hpp"
#include "Rectangle.hpp"
#include "PhysicsMath.hpp"
#include "Physics.hpp"
#include "Quadtree.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/Window/Clipboard.hpp>
#include <UserInterface.hpp>
#include <stack>

#define shptr_obj shared_ptr<Object>
#define shptr_rect shared_ptr<Rectangle>
#define shptr_circ shared_ptr<Circle>

using Vec2 = syxd::Vector2<float>;
using std::cout;
using std::endl; 
using std::string;
using std::shared_ptr; 
using std::unique_ptr; 
using std::weak_ptr;
using std::vector;

struct WINDOW_SETTINGS {
  float MAX_FRAME_RATE;
  uint16_t DEFAULT_WINDOW_SIZE_X;
  uint16_t DEFAULT_WINDOW_SIZE_Y;
  float WORLD_SIZE; // always square
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
  const float ZOOM_AMOUNT = 1.1f;
  
  float _MOVE_SENSITIVITY = 1.0f;
  float _INPUT_INTERVAL = 0.1f;
  float _CURSOR_BLINK_INTERVAL = 0.50f;
  

public:
  shared_ptr<sf::RenderWindow> WINDOW; // Main render window that displays stuff
  sf::Clock clock; // to keep track of time between frames
  sf::View m_main_view; // main view of the world
  sf::View m_ui_view; // ui has a seperate view so that zooming/moving through the world doesn't affect the ui
  sf::Font m_default_font;
  
  sf::Vector2i m_mouse_pos_i;
  sf::Vector2f m_mouse_pos_f;
  
  float m_elapsed_time_spawn = CREATION_INTERVAL;
  float m_elapsed_time_move = INTERRUPT_INTERVAL;
  float m_elapsed_time_input = _INPUT_INTERVAL;
  float m_elapsed_time_cursor_blink = _CURSOR_BLINK_INTERVAL;
  float m_elapsed_diagnostic = TOGGLE_INTERVAL;
  float m_elapsed_time_increase = CREATION_INTERVAL;
  
  bool m_select_mode = false; // multi select mode
  bool m_command_mode = false;
  const float m_default_drag = -0.7f;
  bool m_gizmos_mode = false;
  bool m_gravity_mode = true;
  float m_drag;
  bool isRunning;
  int num_objects_to_spawn = 1;
  
  std::unique_ptr<Quadtree> m_quad_root;
  
  WINDOW_SETTINGS m_window_settings;
  UI_SETTINGS m_ui_settings; 

  UserInterface m_user_interface;
  
  Engine( const uint16_t world_size );
  ~Engine();

  void EventManager( const float& delta_time ); // event and input manager
  void CollisionCheck( ); // checks if any collision has occured and provides a response to that collision 
  void UpdatePhysics( const float& delta_time ); // updates object data
  void Render( ); // render any non-ui and non-world elements
  void UI( ); // UI
  
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
  void displayGizmos( );
  void InitializeWorld( );
  void displayDiagnosticInfo( const std::chrono::high_resolution_clock::time_point& start,
                              const uint64_t& cpu_usage, 
                              const uint64_t& memory_available,
                              const uint64_t& memory_used); // returns window frames per second
  void InitializeUI( );
  void setZoomLimits( const sf::Vector2f& worldSize, const sf::Vector2f& windowSize );
  
  void checkCollisionWithWorld( const shptr_obj object ) const noexcept;
  void drawEdges( );
};
