
#include <cassert>
#include "Level.hpp"

UnitAction _getPlayerAction(sf::Keyboard::Key key) {
  UnitAction::Direction dir;

  switch (key) {
    case sf::Keyboard::Up: {
      dir = UnitAction::Direction::Up;
      break;
    }
    case sf::Keyboard::Right: {
      dir = UnitAction::Direction::Right;
      break;
    }
    case sf::Keyboard::Down: {
      dir = UnitAction::Direction::Down;
      break;
    }
    case sf::Keyboard::Left: {
      dir = UnitAction::Direction::Left;
      break;
    }
    default: {
      assert(0);
    }
  }
  return { UnitAction::Type::MoveAction, dir };
}

void Level::playerMove(sf::Keyboard::Key key) {
  std::vector<UnitAction> actions(m_units.size());

  actions[0] = { _getPlayerAction(key) };

  AnimationState nextAnimations;
  nextAnimations.unitAnimations.resize(m_units.size(), {});
  for (size_t i = 0; i < actions.size(); ++i) {
    nextAnimations.unitAnimations[i].action = actions[i];
  }

  m_nextAnimations = { nextAnimations };
  m_msTimeUntilNext = 0;
}
