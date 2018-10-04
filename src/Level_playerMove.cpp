
#include <cassert>
#include "Level.hpp"

UnitAction _getPlayerAction(sf::Keyboard::Key key) {
  Direction dir;

  switch (key) {
    case sf::Keyboard::Up: {
      dir = Direction::Up;
      break;
    }
    case sf::Keyboard::Right: {
      dir = Direction::Right;
      break;
    }
    case sf::Keyboard::Down: {
      dir = Direction::Down;
      break;
    }
    case sf::Keyboard::Left: {
      dir = Direction::Left;
      break;
    }
    default: {
      assert(0);
    }
  }
  return { UnitAction::Type::MoveAction, dir };
}

void Level::onPlayerMove(sf::Keyboard::Key key) {
  std::vector<UnitAction> actions(m_units.size());
  AnimationState nextState;

  // MOVE PLAYER
  actions[0] = { _getPlayerAction(key) };

  auto nextPlayerPos = m_world.player.pos + getDeltaPosFromDir(actions[0].dir);
  if (WorldState::isSolid(m_world.tiles.get(nextPlayerPos))) {
    actions[0].type = UnitAction::Type::BumpAction;
    nextPlayerPos = m_world.player.pos;
  }

  // MOVE ENEMIES
  for (size_t i = 0; i < m_world.guards.size(); ++i) {
    actions[i + 1] = nextGuardMovement(
      WorldState::getDistances(m_world.tiles, nextPlayerPos), i
    );
  }

  // HANDLE TERRAIN
  nextState.tiles = m_world.tiles;
  for (size_t i = 0; i < actions.size(); ++i) {
    const WorldState::Unit& unit = m_units[i];

    if (actions[i].type == UnitAction::Type::BumpAction) {
      Pos bumpPos = *unit.pos + getDeltaPosFromDir(actions[i].dir);
      WorldState::triggerTile(nextState.tiles, bumpPos);
    }
  }

  // CLOSE DOORS
  // TODO

  // HANDLE INTER-UNIT COLLISIONS
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

  // UPDATE LOS
  // TODO

  // SPREAD ANGRY
  // TODO

  // ADD MOVEMENT ANIMATIONS
  nextState.unitAnimations.resize(m_units.size(), {});
  for (size_t i = 0; i < actions.size(); ++i) {
    nextState.unitAnimations[i].action = actions[i];
  }

  m_nextAnimations = { nextState };
  m_msTimeUntilNext = 0;
}
