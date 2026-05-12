#include <Window.hpp>

syxd::Window::Window( WINDOW_SETTINGS window_settings, std::function<UserInterface()> init_ui_function ) {
  InitializeUI = init_ui_function;
  functions.reserve(DEFAULT_NUM_FUNCTIONS);

  if (InitializeUI)
    InitializeWindow( window_settings );    
    m_main_ui = std::move(InitializeUI());

  assert(m_main_ui);

};

syxd::Window::~Window( ) {

};

void Window::InitializeWindow( WINDOW_SETTINGS window_settings ){
  m_MAIN_WINDOW = std::make_shared<sf::RenderWindow>();
  m_MAIN_WINDOW->create( sf::VideoMode( window_settings.DEFAULT_WINDOW_SIZE_X, window_settings.DEFAULT_WINDOW_SIZE_Y ), window_settings.WINDOW_NAME );
  m_MAIN_WINDOW->setFramerateLimit( static_cast<unsigned int>(window_settings.MAX_FRAME_RATE) );
  m_MAIN_WINDOW->setView( m_main_view = sf::View( sf::FloatRect( 0.0f, 0.0f, static_cast<float>(window_settings.DEFAULT_WINDOW_SIZE_X), static_cast<float>(window_settings.DEFAULT_WINDOW_SIZE_Y) ) ) );
  m_MAIN_WINDOW->setView( m_ui_view = sf::View( sf::FloatRect( 0.0f, 0.0f, static_cast<float>(window_settings.DEFAULT_WINDOW_SIZE_X), static_cast<float>(window_settings.DEFAULT_WINDOW_SIZE_Y) ) ) );

  m_main_ui.SetWindow( m_MAIN_WINDOW );
  m_window_settings = window_settings;
  half_size = sf::Vector2f( m_MAIN_WINDOW->getSize().x/2, m_MAIN_WINDOW->getSize().y/2 );

  #if DEBUG
  if ( !m_default_font.loadFromFile( fonts[3] ) ){
      DEBUG_PRINT("Font not found\n");
  }
  else {
      DEBUG_PRINT("Font loaded\n");

  }
  
  #else
    if ( !m_default_font.loadFromFile( "static/fonts/cairo.ttf" ) ){
        DEBUG_PRINT("Font not found\n");
    }
  #endif

  m_main_ui.SetFont( m_default_font );
  m_background_color = sf::Color::Black;

}

std::shared_ptr<sf::RenderWindow> Window::getRenderWindow(){
  return m_MAIN_WINDOW;
}

void Window::setRenderWindow( std::shared_ptr<sf::RenderWindow> window ) {
  m_MAIN_WINDOW = window;
}

sf::View Window::getMainView(){
  return m_main_view;
}

void Window::setMainView( sf::View updated_view ){
  m_main_view = updated_view;
}

sf::View Window::getUIView(){
  return m_ui_view;
}

void Window::setUIView( sf::View updated_view ){
  m_ui_view = updated_view;
}

UserInterface& Window::getUserInterface() {
  return m_main_ui;
}

void Window::setUserInterface( UserInterface& updated_UI ){
  m_main_ui = std::move(updated_UI);
}

void Window::setInitilizeUIFunctionUI( std::function<UserInterface()> updated_function ){
  InitializeUI = updated_function;
}


void Window::addFunction( std::function<void()> new_function ){
  functions.push_back(new_function);
}

void Window::EventManager( const float& delta_time ) {
  if( m_MAIN_WINDOW != nullptr && m_MAIN_WINDOW->pollEvent( e_event ) ) {
    switch( e_event.type ) {
      case sf::Event::Closed:
        m_MAIN_WINDOW->close();
      break;

      case sf::Event::Resized:
        float prev_width = m_main_view.getSize().x;
        float prev_height = m_main_view.getSize().y;
        m_MAIN_WINDOW->setView( m_main_view = sf::View( sf::FloatRect( 0.0f, 0.0f, e_event.size.width, e_event.size.height ) ) );
        m_main_ui.resizeUI( sf::Vector2f( prev_width, prev_height ), sf::Vector2u( e_event.size.width, e_event.size.height ) );
      break;

    }
  }
}

void Window::WindowLoop( ) {

  while ( m_MAIN_WINDOW->isOpen() ) {
    m_MAIN_WINDOW->clear( m_background_color );

    const float& delta_time = clock.restart().asSeconds();

    EventManager( delta_time );
    
    for (auto& ext_func : functions){
      if (ext_func) ext_func();
    }

    if ( m_MAIN_WINDOW != NULL ){
      m_MAIN_WINDOW->setView( m_ui_view ); // setting view for UI, so that UI does not change size when moving / zooming in world
      m_main_ui.RenderUI( delta_time );
      m_MAIN_WINDOW->setView( m_main_view ); // resetting view to main 
      m_MAIN_WINDOW->display();
    }
  }
}