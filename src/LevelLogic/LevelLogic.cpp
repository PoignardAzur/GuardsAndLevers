#include <cstdio>
#include <cassert>
#include "LevelLogic.hpp"

LevelLogic::LevelLogic(WorldState state) {
  this->world = state;

  this->units = this->world.getUnits();
  this->guardCount = this->world.guards.size();

  this->individualLosTokens.resize(this->guardCount);
  updateLos();
  updatePathfinding();
}

static bool _canSeePlayerOrAngryGuard(
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

std::deque<ActionState> LevelLogic::onPlayerMove(PlayerAction playerAction) {
  ActionState moveActions(this->guardCount);
  WorldState nextState = this->world;
  std::deque<ActionState> nextActions;
  nextActions = {};

  // MOVE PLAYER
  Pos nextPlayerPos = this->world.player.pos;
  if (playerAction.type == MoveType::Move) {
    nextPlayerPos += getDeltaPosFromDir(playerAction.dir);
  }
  if (WorldState::isSolid(this->world.tiles.get(nextPlayerPos))) {
    playerAction.type = MoveType::Bump;
    nextPlayerPos = this->world.player.pos;
  }
  moveActions.playerAction = playerAction;

  // MOVE ENEMIES
  for (size_t i = 0; i < this->guardCount; ++i) {
    moveActions.guardActions[i] = nextGuardMovement(
      WorldState::getDistances(this->world.tiles, nextPlayerPos), i
    );
  }

  // HANDLE TERRAIN
  nextState.tiles = this->world.tiles;
  auto allActions = moveActions.getAllActions();
  for (size_t i = 0; i < allActions.size(); ++i) {
    const UnitState* unit = this->units[i];

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
      const UnitState* unit1 = this->units[i];
      Pos nextPos1 = unit1->pos;

      if (allActions[i]->type == MoveType::Move) {
        nextPos1 += getDeltaPosFromDir(allActions[i]->dir);
      }

      for (size_t j = 0; j < allActions.size(); ++j) {
        const UnitState* unit2 = this->units[j];

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
  nextActions.push_back(moveActions);
  moveActions.applyChanges(nextState);

  // SPREAD ANGRY
  std::vector<Grid2<char>> nextLosTokens(this->guardCount);
  for (size_t i = 0; i < this->guardCount; ++i) {
    nextLosTokens[i] = WorldState::generateLosTokens(
      nextState.tiles, nextState.guards[i]
    );
  }

  bool checkLosAgain = true;
  bool isPlayerDead = false;
  while (checkLosAgain && !isPlayerDead) {
    checkLosAgain = false;

    ActionState angryGuardActions(this->guardCount);

    for (size_t i = 0; i < this->guardCount; ++i) {
      GuardState& guard = nextState.guards[i];

      if (guard.isAngry) {
        if (auto dir = guard.isNextTo(nextState.player.pos)) {
          // TODO - GetCaught action
          // TODO - HitPlayer action
          angryGuardActions.playerAction = {{ MoveType::Bump, Direction::Up }};
          angryGuardActions.guardActions[i] = {{ MoveType::Bump, *dir }};
          isPlayerDead = true;
        }
      }
      if (!guard.isAngry && _canSeePlayerOrAngryGuard(nextState, i, nextLosTokens[i])) {
        angryGuardActions.guardActions[i] = {{ MoveType::GetAngry, Direction::Up }};
        // FIXME
        checkLosAgain = true;
      }
    }

    if (isPlayerDead || checkLosAgain) {
      nextActions.push_back(angryGuardActions);
      angryGuardActions.applyChanges(nextState);
    }
  }

  return nextActions;
}

void LevelLogic::updateLos() {
  this->collectiveLosTokens = Grid2<char>(this->world.tiles.getSize(), 0);

  for (size_t i = 0; i < this->guardCount; ++i) {
    this->individualLosTokens[i] = WorldState::generateLosTokens(
      this->world.tiles, this->world.guards[i]
    );

    for (long x = 0; x < this->world.tiles.getSize().x; ++x) {
      for (long y = 0; y < this->world.tiles.getSize().y; ++y) {
        this->collectiveLosTokens.set(
          x, y,
          this->collectiveLosTokens.get(x, y)
          || this->individualLosTokens[i].get(x, y)
        );
      }
    }
  }
}

void LevelLogic::updatePathfinding() {
  for (const GuardState& guard : this->world.guards) {
    for (Pos patrolStop : guard.patrolStops) {
      if (this->pathfindings.count(patrolStop) != 0) {
        continue;
      }
      this->pathfindings[patrolStop] = WorldState::getDistances(
        this->world.tiles, patrolStop
      );
    }
  }
}
