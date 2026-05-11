#include <Window.hpp>

syxd::Window::Window( WINDOW_SETTINGS window_settings ) {
  m_MAIN_WINDOW = std::make_shared<sf::RenderWindow>();
  m_MAIN_WINDOW->create( sf::VideoMode( window_settings.DEFAULT_WINDOW_SIZE_X, window_settings.DEFAULT_WINDOW_SIZE_Y ), window_settings.WINDOW_NAME );
  m_MAIN_WINDOW->setFramerateLimit( static_cast<unsigned int>(window_settings.MAX_FRAME_RATE) );
  m_MAIN_WINDOW->setView( m_main_view = sf::View( sf::FloatRect( 0.0f, 0.0f, static_cast<float>(window_settings.DEFAULT_WINDOW_SIZE_X), static_cast<float>(window_settings.DEFAULT_WINDOW_SIZE_Y) ) ) );
 
  m_main_ui.SetWindow( m_MAIN_WINDOW );
  m_window_settings = window_settings;
  half_size = sf::Vector2f( m_MAIN_WINDOW->getSize().x/2, m_MAIN_WINDOW->getSize().y/2 );
};

syxd::Window::~Window( ) {

};

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

UserInterface& Window::getUserInterface() {
  return m_main_ui;
}

void Window::setUserInterface( UserInterface updated_UI ){

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
  m_main_ui.InitText("", "Hello World", 30, sf::Vector2f(m_window_settings.DEFAULT_WINDOW_SIZE_X / 2, m_window_settings.DEFAULT_WINDOW_SIZE_Y / 2), sf::Color::White);

  while ( m_MAIN_WINDOW->isOpen() ) {
    m_MAIN_WINDOW->clear( m_background_color );

    const float& delta_time = clock.restart().asSeconds();

    EventManager( delta_time );
    
    m_main_ui.RenderUI( delta_time );

    m_MAIN_WINDOW->display();
  }
}