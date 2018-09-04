
#ifndef UNIT_ANIMATION_HPP
#define UNIT_ANIMATION_HPP

#include <ctime>
#include <SFML/System/Vector2.hpp>
#include "UnitAction.hpp"

struct UnitAnimation {
  sf::Vector2f getDeltaPos(time_t msAnimTime) const;
  time_t msDuration() const;

  UnitAction action;
  time_t msLifeTime = 0;
};

#endif // !UNIT_ANIMATION_HPP
