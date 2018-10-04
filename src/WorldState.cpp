
#include <vector>
#include <deque>
#include <algorithm>
#include <climits>
#include <cassert>
#include "WorldState.hpp"
#include "UnitAction.hpp"

bool WorldState::isSolid(const Grid2<Tile>& tilemap, Pos tilePos) {
  switch (tilemap.get(tilePos)) {
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

    UnitAction::Direction directions[] = {
      UnitAction::Direction::Up,
      UnitAction::Direction::Right,
      UnitAction::Direction::Down,
      UnitAction::Direction::Left
    };

    for (UnitAction::Direction dir : directions) {
      auto otherNode = closestNode + getDeltaPosFromDir(dir);

      if (!isSolid(tilemap, otherNode)) {
        distances.set(otherNode, std::min(
          distances.get(otherNode), distances.get(closestNode) + 1
        ));
      }
    }

    unvisitedNodes.pop_front();
  }
  return distances;
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
