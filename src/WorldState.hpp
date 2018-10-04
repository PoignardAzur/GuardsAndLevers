
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

enum class Direction {
  Up,
  Right,
  Down,
  Left
};

struct PlayerState {
  Pos pos;
};

struct GuardState {
  Pos pos;
  Direction dir;
  std::vector<Pos> patrolStops;
  size_t nextStopId = 0;
  bool isAngry = false;
};

struct WorldState {
  struct Unit {
    Pos* pos;
    sf::Color color;
  };

  static bool isSolid(Tile tile);
  static bool blocksLos(Tile tile);
  static Grid2<int> getDistances(const Grid2<Tile>& tilemap, Pos tilePos);
  static Grid2<char> generateLosTokens(const Grid2<Tile>& tilemap, const GuardState& guard);
  static void triggerTile(Grid2<Tile>& tilemap, Pos triggeredTile);

  std::vector<Unit> getUnits();

  Grid2<Tile> tiles;
  std::vector<GuardState> guards = {};
  PlayerState player = {};
};

Pos getDeltaPosFromDir(Direction dir);

#endif // !WORLD_STATE_HPP
