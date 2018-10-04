
#include <climits>
#include <cassert>
#include "Level.hpp"

#include <cstdio>

UnitAction Level::nextGuardMovement(const Grid2<int>& distancesToPlayer, size_t i) {
  assert(i < m_world.guards.size());

  GuardState& guard = m_world.guards[i];
  const Grid2<int>* pathfindingDistances = nullptr;

  if (guard.isAngry) {
    pathfindingDistances = &distancesToPlayer;
  }
  else {
    if (guard.pos == guard.patrolStops[guard.nextStopId]) {
      // TODO - Rework with functional logic?
      guard.nextStopId++;
      guard.nextStopId %= guard.patrolStops.size();
    }
    Pos nextStop = guard.patrolStops[guard.nextStopId];
    pathfindingDistances = &m_pathfindings.find(nextStop)->second;
  }

  Direction directions[] = {
    Direction::Up,
    Direction::Right,
    Direction::Down,
    Direction::Left
  };
  int minDistance = INT_MAX;
  Direction selectedDir = Direction::Up;
  for (Direction dir : directions) {
    auto selectedPos = guard.pos + getDeltaPosFromDir(dir);
    if (pathfindingDistances->get(selectedPos) < minDistance) {
      selectedDir = dir;
      minDistance = pathfindingDistances->get(selectedPos);
    }
  }
  // TODO - Bump into levers
  return { UnitAction::Type::MoveAction, selectedDir };
}
