
#include <cassert>
#include "Level.hpp"

// TODO - use in onKeyPressed
PlayerAction _getPlayerAction(sf::Keyboard::Key key) {
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
  return {{ MoveType::Move, dir }};
}

bool _canSeePlayerOrAngryGuard(
  const WorldState& worldState,
  size_t guardId,
  const Grid2<char>& losTokens
) {
  if (losTokens.get(worldState.player.pos)) {
    return true;
  }
  for (size_t i = 0; i < worldState.guards.size(); ++i) {
    const GuardState& otherGuard = worldState.guards[i];
    if (i != guardId) {
      if (otherGuard.isAngry && losTokens.get(otherGuard.pos)) {
        return true;
      }
    }
  }
  return false;
}

void Level::onPlayerMove(sf::Keyboard::Key key) {
  ActionState moveActions(m_world.guards.size());
  WorldState nextState = m_world;
  m_nextActions = {};
  m_msTimeUntilNext = 0;

  // MOVE PLAYER
  moveActions.playerAction = _getPlayerAction(key);

  auto nextPlayerPos = m_world.player.pos + getDeltaPosFromDir(moveActions.playerAction.dir);
  if (WorldState::isSolid(m_world.tiles.get(nextPlayerPos))) {
    moveActions.playerAction.type = MoveType::Bump;
    nextPlayerPos = m_world.player.pos;
  }

  // MOVE ENEMIES
  for (size_t i = 0; i < m_world.guards.size(); ++i) {
    moveActions.guardActions[i] = nextGuardMovement(
      WorldState::getDistances(m_world.tiles, nextPlayerPos), i
    );
  }

  // HANDLE TERRAIN
  nextState.tiles = m_world.tiles;
  auto allActions = moveActions.getAllActions();
  for (size_t i = 0; i < allActions.size(); ++i) {
    const UnitState* unit = m_units[i];

    if (allActions[i]->type == MoveType::Move) {
      Pos nextPos = unit->pos + getDeltaPosFromDir(allActions[i]->dir);
      if (WorldState::isTrigger(nextState.tiles.get(nextPos))) {
        WorldState::triggerTile(nextState.tiles, nextPos);
        allActions[i]->type = MoveType::Bump;
      }
    }
  }

  // CLOSE DOORS
  // TODO

  // HANDLE INTER-UNIT COLLISIONS
  bool checkCollisions = true;
  while (checkCollisions) {
    checkCollisions = false;
    for (size_t i = 0; i < allActions.size(); ++i) {
      const UnitState* unit1 = m_units[i];
      Pos nextPos1 = unit1->pos;

      if (allActions[i]->type == MoveType::Move) {
        nextPos1 += getDeltaPosFromDir(allActions[i]->dir);
      }

      for (size_t j = 0; j < allActions.size(); ++j) {
        const UnitState* unit2 = m_units[j];

        if (allActions[j]->type == MoveType::Move) {
          Pos nextPos2 = unit2->pos + getDeltaPosFromDir(allActions[j]->dir);

          if (i != j && nextPos1 == nextPos2) {
            allActions[j]->type = MoveType::Bump;
          }
        }
      }
    }
  }

  // APPLY MOVEMENTS
  m_nextActions.push_back(moveActions);
  moveActions.applyChanges(nextState);

  // SPREAD ANGRY
  std::vector<Grid2<char>> nextLosTokens(nextState.guards.size());
  for (size_t i = 0; i < nextState.guards.size(); ++i) {
    nextLosTokens[i] = WorldState::generateLosTokens(
      nextState.tiles, nextState.guards[i]
    );
  }

  bool checkLosAgain = true;
  while (checkLosAgain && !nextState.player.isDead) {
    checkLosAgain = false;

    ActionState nextActions(m_world.guards.size());

    for (size_t i = 0; i < nextState.guards.size(); ++i) {
      GuardState& guard = nextState.guards[i];

      if (guard.isAngry) {
        if (auto dir = guard.isNextTo(nextState.player.pos)) {
          // TODO - GetCaught action
          // TODO - HitPlayer action
          nextActions.playerAction = {{ MoveType::Bump, Direction::Left }};
          nextActions.guardActions[i] = {{ MoveType::Bump, *dir }};
        }
      }
      if (!guard.isAngry && _canSeePlayerOrAngryGuard(nextState, i, nextLosTokens[i])) {
        // TODO - GetAngry action
        nextActions.guardActions[i] = {{ MoveType::Bump, Direction::Up }};
        // FIXME
        guard.isAngry = true;
        checkLosAgain = true;
      }
    }

    if (nextState.player.isDead || checkLosAgain) {
      m_nextActions.push_back(nextActions);
      nextActions.applyChanges(nextState);
    }
  }

  // TODO - return actions instead of setting them
}
