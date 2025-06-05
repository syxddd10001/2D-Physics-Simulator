#include <UIElements.hpp>
#include <iostream>


syxd::UI_Element::UI_Element ( const uint32_t id, const std::string identifier, const sf::Vector2f position ) noexcept
  : m_id(id), m_identifier(identifier), m_position(position)
{
  std::cout << id << " created UI Elem\n";

}