
#ifndef UNIT_ANIMATION_HPP
#define UNIT_ANIMATION_HPP

#include <ctime>
#include <SFML/System/Vector2.hpp>
#include "UnitAction.hpp"

struct UnitAnimation {
  UnitAction action;

  sf::Vector2f getDeltaPos(time_t msAnimTime) const;
  time_t msDuration() const;
};

#endif // !UNIT_ANIMATION_HPP
