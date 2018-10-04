
#include <vector>
#include <deque>
#include <algorithm>
#include <climits>
#include <cassert>
#include "WorldState.hpp"
#include "UnitAction.hpp"

bool WorldState::isSolid(Tile tile) {
  switch (tile) {
    case Tile::Ground:
    return false;
    case Tile::Wall:
    return true;
    case Tile::OpenDoor:
    return false;
    case Tile::ClosedDoor:
    return true;
    case Tile::TileCount:
    break;
  }
  assert(0);
}

bool WorldState::blocksLos(Tile tile) {
  switch (tile) {
    case Tile::Ground:
    return false;
    case Tile::Wall:
    return true;
    case Tile::OpenDoor:
    return false;
    case Tile::ClosedDoor:
    return true;
    case Tile::TileCount:
    break;
  }
  assert(0);
}

Grid2<int> WorldState::getDistances(const Grid2<Tile>& tilemap, Pos target) {
  Grid2<int> distances(tilemap.getSize(), INT_MAX);
  std::deque<Pos> unvisitedNodes;

  for (long x = 0; x < tilemap.getSize().x; ++x) {
    for (long y = 0; y < tilemap.getSize().y; ++y) {
      unvisitedNodes.push_back({ x, y });
    }
  }
  distances.set(target, 0);

  while (!unvisitedNodes.empty()) {
    std::sort(
      unvisitedNodes.begin(), unvisitedNodes.end(),
      [&](Pos p1, Pos p2) { return distances.get(p1) < distances.get(p2); }
    );

    Pos closestNode = unvisitedNodes[0];
    if (distances.get(closestNode) == INT_MAX) {
      break;
    }

    Direction directions[] = {
      Direction::Up,
      Direction::Right,
      Direction::Down,
      Direction::Left
    };

    for (Direction dir : directions) {
      auto otherNode = closestNode + getDeltaPosFromDir(dir);

      if (!isSolid(tilemap.get(otherNode))) {
        distances.set(otherNode, std::min(
          distances.get(otherNode), distances.get(closestNode) + 1
        ));
      }
    }

    unvisitedNodes.pop_front();
  }
  return distances;
}

Grid2<char> WorldState::generateLosTokens(const Grid2<Tile>& tilemap, const GuardState& guard) {
  Grid2<char> tokens(tilemap.getSize(), 0);

  Pos forwardStep = getDeltaPosFromDir(guard.dir);
  Pos rightStep = getDeltaPosFromDir((Direction)(((int)guard.dir + 1) % 4));
  Pos leftStep = -rightStep;

  Pos projections[] = { leftStep, { 0, 0 }, rightStep };
  for (Pos projection : projections) {
    Pos currentPos = guard.pos + projection - forwardStep;
    while (!WorldState::blocksLos(tilemap.get(currentPos))) {
      tokens.set(currentPos, 1);
      currentPos += forwardStep;
    }
  }

  return tokens;
}

void WorldState::triggerTile(Grid2<Tile>& tilemap, Pos triggeredTile) {
  switch (tilemap.get(triggeredTile)) {
    default:
    break;
  }
}

std::vector<WorldState::Unit> WorldState::getUnits() {
  std::vector<WorldState::Unit> units(1 + this->guards.size());

  units[0] = { &this->player.pos, sf::Color::Green };
  for (size_t i = 0; i < this->guards.size(); ++i) {
    units[i + 1] = { &this->guards[i].pos, sf::Color::Red };
  }
  return units;
}

Pos getDeltaPosFromDir(Direction dir) {
  switch (dir) {
    case Direction::Up:
      return { 0, -1 };
    case Direction::Right:
      return { 1, 0 };
    case Direction::Down:
      return { 0, 1 };
    case Direction::Left:
      return { -1, 0 };
  }
  assert(0);
}
