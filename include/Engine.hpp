#pragma once
#include <vector>
#include <memory>
#include <utility>
#include <thread>
#include <iostream>
#include <cmath>
#include <locale>
#include <conio.h>
#include <stdlib.h>
#include <assert.h>

#include "Rectangle.hpp"
#include "Object.hpp"
#include "PhysicsMath.hpp"
#include "Physics.hpp"


class Engine{
protected:
  const float CREATION_INTERVAL = 0.2f;
  const float INTERRUPT_INTERVAL = 0.01f; 
  const float ZOOM_AMOUNT = 1.1f;
  float MOVE_SENSITIVITY = 1.f;
  float INPUT_INTERVAL = 0.1f;
  float CURSOR_BLINK_INTERVAL = 0.50f;

public:
  std::shared_ptr<sf::RenderWindow> WINDOW; // Main render window that displays stuff
  sf::Clock clock; // to keep track of time between frames
  sf::View mainView; // main view of the world
  sf::View UIView; // ui has a seperate view so that zooming/moving through the world doesn't affect the ui
  sf::Font default_font;
    
  sf::Vector2i mousePos;
  sf::Vector2f mousePosf;
  
  float elapsed_time_spawn = CREATION_INTERVAL;
  float elapsed_time_move = INTERRUPT_INTERVAL;
  float elapsed_time_input = INPUT_INTERVAL;
  float elapsed_time_cursor_blink = CURSOR_BLINK_INTERVAL;

  bool select_mode = false; // multi select mode
  bool command_mode = false;
  const float default_friction = -0.7f;

  float friction;

  Engine( );
  ~Engine();
  void Update( const float* delta_time ); // updates objects and draws it on the screen
  void EventManager( ); // event and input manager
  void addObject( Object* object ); // adds a new object to the world
  std::vector<Object*>* GetAllObjects( ); // returns all objects in the world
  void zoomViewAt( sf::Vector2i pixel, float zoom ); // zoom
  void collisionCheck( ); // checks if any collision has occured and provides a response to that collision 
  void Render( ); // render any non-ui and non-world elements
  void displayFramesPerSecond( std::chrono::high_resolution_clock::time_point start ); // returns window frames per second
  void UI( ); // UI
  void GetObjectsInArea( const point start, const point rect_size ); // returns all objects in a selected area
  void DragRectangle( ); // draws a rectangle to select objects in an area
  void objectDefault( ); // returns objects its default configuration
  void moveSelection( const sf::Vector2f delta ); // moves selected objects
  void checkObjectsSelected( ); // check if an object is selected from a specified area
  void moveAll( std::vector<Object*>* objects, const sf::Vector2f delta ); // move all objects that are selected
  void deleteObject( Object* object_to_delete, std::vector<Object*>& all_objects ); // delete a selected object
  void deleteSelectedObjects( std::vector<Object*>& all_objects ); // deleted all objects that are selected

};

