
#ifndef UNIT_ANIMATION_HPP
#define UNIT_ANIMATION_HPP

#include <SFML/Vector2.hpp>
#include "UnitAction.hpp"

struct UnitAnimation {
  UnitAction action;

  sf::Vector2f getDeltaPos() const;
};

#endif // !UNIT_ANIMATION_HPP
