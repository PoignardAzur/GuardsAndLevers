
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

  // MOVE PLAYER
  actions[0] = { _getPlayerAction(key) };

  // MOVE ENEMIES
  for (size_t i = 0; i < m_world.guards.size(); ++i) {
    actions[i + 1] = guardAi(i);
  }

  // HANDLE COLLISIONS
  bool checkCollisions = true;
  while (checkCollisions) {
    checkCollisions = false;
    for (size_t i = 0; i < actions.size(); ++i) {
      const WorldState::Unit& unit1 = m_units[i];
      Pos nextPos1 = *unit1.pos;

      if (actions[i].type == UnitAction::Type::MoveAction) {
        nextPos1 += getDeltaPosFromDir(actions[i].dir);
      }

      for (size_t j = 0; j < actions.size(); ++j) {
        const WorldState::Unit& unit2 = m_units[j];

        if (actions[j].type == UnitAction::Type::MoveAction) {
          Pos nextPos2 = *unit2.pos + getDeltaPosFromDir(actions[j].dir);

          if (i != j && nextPos1 == nextPos2) {
            actions[j].type = UnitAction::Type::BumpAction;
          }
        }
      }
    }
  }

  // ADD MOVEMENT ANIMATIONS
  AnimationState nextAnimations;
  nextAnimations.unitAnimations.resize(m_units.size(), {});
  for (size_t i = 0; i < actions.size(); ++i) {
    nextAnimations.unitAnimations[i].action = actions[i];
  }

  m_nextAnimations = { nextAnimations };
  m_msTimeUntilNext = 0;
}
