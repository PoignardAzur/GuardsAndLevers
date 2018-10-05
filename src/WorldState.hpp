
#ifndef WORLD_STATE_HPP
#define WORLD_STATE_HPP

#include <vector>

#include <SFML/Graphics.hpp>
#include "Grid2.hpp"

enum class Direction;

enum class Tile : size_t {
  Ground,
  Wall,
  OpenDoor,
  ClosedDoor,
  TileCount
};

struct UnitState {
  Pos pos;
  Direction dir;
};

struct PlayerState : public UnitState {
  bool isDead = false;
};

struct GuardState : public UnitState {
  std::vector<Pos> patrolStops;
  size_t nextStopId = 0;
  bool isAngry = false;
};

struct WorldState {
  static bool isSolid(Tile tile);
  static bool isTrigger(Tile tile);
  static bool blocksLos(Tile tile);
  static Grid2<int> getDistances(const Grid2<Tile>& tilemap, Pos tilePos);
  static Grid2<char> generateLosTokens(const Grid2<Tile>& tilemap, const GuardState& guard);
  static void triggerTile(Grid2<Tile>& tilemap, Pos triggeredTile);

  std::vector<UnitState*> getUnits();

  Grid2<Tile> tiles;
  std::vector<GuardState> guards = {};
  PlayerState player = {};
};

#endif // !WORLD_STATE_HPP
