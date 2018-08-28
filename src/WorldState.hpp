
#ifndef WORLD_STATE_HPP
#define WORLD_STATE_HPP

#include <memory>
#include <vector>

struct Pos {
  long x;
  long y;
};

enum class Tile : size_t {
  Ground,
  Wall,
  OpenDoor,
  ClosedDoor,
  TileCount
};

class Tilemap
{
public:
  Tilemap(Pos size, Tile fillTile = Tile::Ground);
  Tilemap(const Tilemap& other) = default;
  Tilemap(Tilemap&& other);
  Tilemap& operator=(const Tilemap& other) = default;
  Tilemap& operator=(Tilemap&& other);

  Tile getTile(Pos pos) const;
  Tile getTile(long x, long y) const;
  void setTile(Pos pos, Tile newTile);
  void setTile(long x, long y, Tile newTile);

  Pos getSize() const;

  using Ptr = std::unique_ptr<Tilemap>;

private:
  std::vector<Tile> m_tileIds;
  Pos m_size;
};

struct PlayerState {
  Pos pos;
};

struct GuardState {
  Pos pos;
};

struct WorldState {
  Tilemap tiles;
  std::vector<GuardState> guards = {};
  PlayerState player = {};
};

#endif // !WORLD_STATE_HPP
