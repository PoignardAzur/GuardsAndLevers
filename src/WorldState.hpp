
#ifndef WORLD_STATE_HPP
#define WORLD_STATE_HPP

#include <memory>
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
  Grid2<Tile> tiles;
  std::vector<GuardState> guards = {};
  PlayerState player = {};

  struct Unit {
    const Pos* pos;
    sf::Color color;
  };

  std::vector<Unit> getUnits() const;
};

#endif // !WORLD_STATE_HPP
