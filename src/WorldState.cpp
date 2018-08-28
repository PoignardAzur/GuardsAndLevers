
#include <cassert>
#include "WorldState.hpp"

Tilemap::Tilemap(Pos size, Tile fillTile) :
  m_tileIds(size.x * size.y, fillTile),
  m_size(size)
{

}

Tilemap::Tilemap(Tilemap&& other) :
  m_tileIds(std::move(other.m_tileIds)),
  m_size(other.m_size)
{
  other.m_size = {0, 0};
}

Tilemap& Tilemap::operator=(Tilemap&& other)
{
  m_tileIds = std::move(other.m_tileIds);
  m_size = other.m_size;
  other.m_size = {0, 0};

  return *this;
}

Tile Tilemap::getTile(Pos pos) const
{
  return getTile(pos.x, pos.y);
}

Tile Tilemap::getTile(long x, long y) const
{
  assert(x < m_size.x && y < m_size.y);
  return m_tileIds[x + y * m_size.y];
}

void Tilemap::setTile(Pos pos, Tile newTile)
{
  setTile(pos.x, pos.y, newTile);
}

void Tilemap::setTile(long x, long y, Tile newTile)
{
  assert(x < m_size.x && y < m_size.y);
  m_tileIds[x + y * m_size.y] = newTile;
}

Pos Tilemap::getSize() const
{
  return m_size;
}
