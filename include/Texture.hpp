#pragma once

#include <utility>
#include <iostream>
#include <memory>
#include <cstdlib>
#include <random>

#include <SFML/Graphics.hpp>
#include "Object.hpp"
#include "PhysicsMath.hpp"
#include "AbstractBox.hpp"
#include "Vector2.hpp"

float getRandomNumber(const float& lowerbound, const float upperbound);
sf::Image createRadialGlowTexture( const float& CORE_RADIUS,
                         const float& HALO_TO_CORE_RATIO, 
                         const float& FALL_EXP,
                         sf::Color* color );