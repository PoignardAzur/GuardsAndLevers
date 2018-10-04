
#ifndef WORLD_STATE_HPP
#define WORLD_STATE_HPP

#include <vector>

#include <SFML/Graphics.hpp>
#include "Grid2.hpp"

enum class Tile : size_t {
  Ground,
  Wall,
  OpenDoor,
  ClosedDoor,
  TileCount
};

struct PlayerState {
  Pos pos;
};

struct GuardState {
  Pos pos;
};

struct WorldState {
  struct Unit {
    Pos* pos;
    sf::Color color;
  };

  static bool isSolid(const Grid2<Tile>& tilemap, Pos tilePos);
  static void triggerTile(Grid2<Tile>& tilemap, Pos triggeredTile);

  std::vector<Unit> getUnits();

  Grid2<Tile> tiles;
  std::vector<GuardState> guards = {};
  PlayerState player = {};
};

#endif // !WORLD_STATE_HPP
