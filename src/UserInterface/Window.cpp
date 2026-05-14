#include <Window.hpp>

syxd::Window::Window( WINDOW_SETTINGS window_settings, std::shared_ptr<UserInterface> user_interface ) {
  functions.reserve(DEFAULT_NUM_FUNCTIONS);
  m_main_ui = user_interface;
  std::cout << m_main_ui << '\n';
  std::cout << user_interface << '\n';


  InitializeWindow( window_settings );  
  
  assert(m_main_ui);

  if (m_MAIN_WINDOW != nullptr){
    m_main_ui->SetWindow(m_MAIN_WINDOW);
  }

};

syxd::Window::~Window( ) {

};

void Window::InitializeWindow( WINDOW_SETTINGS window_settings ){
  m_MAIN_WINDOW = std::make_shared<sf::RenderWindow>();
  m_MAIN_WINDOW->create( sf::VideoMode( window_settings.DEFAULT_WINDOW_SIZE_X, window_settings.DEFAULT_WINDOW_SIZE_Y ), window_settings.WINDOW_NAME );
  m_MAIN_WINDOW->setFramerateLimit( static_cast<unsigned int>(window_settings.MAX_FRAME_RATE) );
  m_MAIN_WINDOW->setView( m_main_view = sf::View( sf::FloatRect( 0.0f, 0.0f, static_cast<float>(window_settings.DEFAULT_WINDOW_SIZE_X), static_cast<float>(window_settings.DEFAULT_WINDOW_SIZE_Y) ) ) );
  m_MAIN_WINDOW->setView( m_ui_view = sf::View( sf::FloatRect( 0.0f, 0.0f, static_cast<float>(window_settings.DEFAULT_WINDOW_SIZE_X), static_cast<float>(window_settings.DEFAULT_WINDOW_SIZE_Y) ) ) );

  if (m_main_ui != nullptr) {
    DEBUG_PRINT("cc");
    m_main_ui->SetWindow( m_MAIN_WINDOW );
    
  }
  

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

  m_main_ui->SetFont( m_default_font );
  
    
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

std::shared_ptr<UserInterface> Window::getUserInterface() {
  return m_main_ui;
}

void Window::setUserInterface( std::shared_ptr<UserInterface> updated_UI ){
  m_main_ui = std::move(updated_UI);
}

void Window::setInitilizeUIFunctionUI( std::function<UserInterface()> updated_function ){
  InitializeUI = updated_function;
}


void Window::addFunction( std::function<void()> new_function ){
  functions.push_back(new_function);
}

void Window::AddEventHandler(std::function<void(const sf::Event&, float, std::shared_ptr<sf::RenderWindow>)> handler) {
  custom_event_handlers.push_back(std::move(handler));
}

void Window::EventManager( const float& delta_time ) {
  if( m_MAIN_WINDOW != nullptr && m_MAIN_WINDOW->pollEvent( e_event ) ) {
    switch( e_event.type ) {
      case sf::Event::Closed:
        m_MAIN_WINDOW->close();
      break;

      case sf::Event::Resized:
      {
        float prev_width = m_ui_view.getSize().x;
        float prev_height = m_ui_view.getSize().y;
        m_MAIN_WINDOW->setView( m_ui_view = sf::View( sf::FloatRect( 0.0f, 0.0f, e_event.size.width, e_event.size.height ) ) );
        m_main_ui->resizeUI( sf::Vector2f( prev_width, prev_height ), sf::Vector2u( e_event.size.width, e_event.size.height ) );
        
        break;
      }

    }

    for (auto& handler : custom_event_handlers) {
      handler( e_event, delta_time, m_MAIN_WINDOW );
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
      m_main_ui->RenderUI( delta_time );
      //m_MAIN_WINDOW->setView( m_main_view ); // resetting view to main 
      m_MAIN_WINDOW->display();
    }

  }
}