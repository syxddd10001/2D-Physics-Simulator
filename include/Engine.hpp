#pragma once
#include <vector>
#include <memory>
#include <utility>
#include <thread>
#include <iostream>
#include <cmath>
#include <stdlib.h>
#include <assert.h>

#include "Vector2.hpp"
#include "Object.hpp"
#include "AbstractBox.hpp"
#include "Rectangle.hpp"
#include "PhysicsMath.hpp"
#include "Physics.hpp"
#include "Quadtree.hpp"

using Vec2 = syxd::Vector2<float>;
using std::cout;
using std::endl; 
using std::string;
using std::shared_ptr; 
using std::unique_ptr; 
using std::weak_ptr;
using std::vector;

#define shared_ptr_obj shared_ptr<Object>
#define shared_ptr_rect shared_ptr<Rectangle>
#define shared_ptr_circ shared_ptr<Circle>

struct WINDOW_SETTINGS {
  float MAX_FRAME_RATE;
  uint32_t DEFAULT_WINDOW_SIZE_X;
  uint32_t DEFAULT_WINDOW_SIZE_Y;
  std::string WINDOW_NAME;
};

struct UI_SETTINGS {
  uint16_t h1_size; // 50
  uint16_t h2_size; // 35
  uint16_t h3_size; // 25
  uint16_t p1_size; // 20
};


class Engine{
private:
  const float CREATION_INTERVAL = 0.1f;
  const float INTERRUPT_INTERVAL = 0.01f; 
  const float ZOOM_AMOUNT = 1.1f;
  
  float MOVE_SENSITIVITY = 1.f;
  float INPUT_INTERVAL = 0.1f;
  float CURSOR_BLINK_INTERVAL = 0.50f;
  

public:
  shared_ptr<sf::RenderWindow> WINDOW; // Main render window that displays stuff
  sf::Clock clock; // to keep track of time between frames
  sf::View main_view; // main view of the world
  sf::View ui_view; // ui has a seperate view so that zooming/moving through the world doesn't affect the ui
  sf::Font default_font;
    
  sf::Vector2i mouse_pos_i;
  sf::Vector2f mouse_pos_f;
  
  float elapsed_time_spawn = CREATION_INTERVAL;
  float elapsed_time_move = INTERRUPT_INTERVAL;
  float elapsed_time_input = INPUT_INTERVAL;
  float elapsed_time_cursor_blink = CURSOR_BLINK_INTERVAL;

  bool select_mode = false; // multi select mode
  bool command_mode = false;
  const float default_drag = -0.7f;
  bool gizmos_mode = false;

  float drag;

  std::unique_ptr<Quadtree> root;
  
  WINDOW_SETTINGS window_settings;
  UI_SETTINGS ui_settings; 

  Engine( );
  ~Engine();
  void EventManager( ); // event and input manager
  void CollisionCheck( ); // checks if any collision has occured and provides a response to that collision 
  void Update( const float* delta_time ); // updates object data
  void Render( ); // render any non-ui and non-world elements
  void UI( ); // UI
  
  void addObject( const shared_ptr_obj object ); // adds a new object to the world
  vector<shared_ptr_obj>& getAllObjects(); // returns all objects in the world
  void zoomViewAt( const sf::Vector2i& pixel, const float& zoom ); // zoom
  void displayFramesPerSecond( const std::chrono::high_resolution_clock::time_point& start ); // returns window frames per second
  void getObjectsInArea( const AbstractBox<float>& rect_size ); // returns all objects in a selected area
  void dragRectangle( ); // draws a rectangle to select objects in an area
  void objectDefault( ); // returns objects its default configuration
  void moveSelection( const sf::Vector2f delta ); // moves selected objects
  void checkObjectsSelected( ); // check if an object is selected from a specified area
  void moveAll( vector<shared_ptr_obj> objects, const sf::Vector2f delta ); // move all objects that are selected
  bool deleteObject( const shared_ptr_obj object_to_delete, vector<shared_ptr<Object>>& all_objects ); // delete a selected object
  void deleteSelectedObjects( vector<shared_ptr_obj>& objects_to_delete ); // deleted all objects that are selected
  void displayGizmos( );
  void InitializeSetup( );
  
};
