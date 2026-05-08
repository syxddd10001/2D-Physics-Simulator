#pragma once
#include <UIElements.hpp>
bool action = false;
syxd::UI_Element::UI_Element ( const uint32_t id, const std::string identifier, const sf::Vector2f position ) noexcept
  : m_id(id), m_identifier(identifier), m_position(position)
{
  DEBUG_PRINT("%s created in UI\n", identifier.c_str());

}
