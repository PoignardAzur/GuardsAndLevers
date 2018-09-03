
#include "WorldState.hpp"

std::vector<WorldState::Unit> WorldState::getUnits() const {
  std::vector<WorldState::Unit> units(1 + this->guards.size());

  units[0] = { &this->player.pos, sf::Color::Green };
  for (size_t i = 0; i < this->guards.size(); ++i) {
    units[i + 1] = { &this->guards[i].pos, sf::Color::Red };
  }
  return units;
}
