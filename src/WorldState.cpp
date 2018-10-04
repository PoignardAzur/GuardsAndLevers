
#include <cassert>
#include "WorldState.hpp"

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
