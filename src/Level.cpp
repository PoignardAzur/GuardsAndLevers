
#include <cmath>
#include <algorithm>
#include <string>

#include "Level.hpp"
#include "units.hpp"

#include <iostream>

Level::Level() :
  m_world({
    Tilemap({10, 10}, Tile::Ground),
    { GuardState {2, 3} },
    PlayerState {3, 3}
  })
{
  for (long i = 0; i < 10; ++i) {
    m_world.tiles.setTile(i, 0, Tile::Wall);
    m_world.tiles.setTile(i, 9, Tile::Wall);
  }
  for (long j = 0; j < 10; ++j) {
    m_world.tiles.setTile(0, j, Tile::Wall);
    m_world.tiles.setTile(9, j, Tile::Wall);
  }

  m_world.tiles.setTile(2, 2, Tile::OpenDoor);
  m_world.tiles.setTile(3, 2, Tile::ClosedDoor);
}

void Level::update(Inputs& inputs) {
}

void Level::display(sf::RenderTarget& window) const {
  window.clear();
  drawWorld(window, m_world);
}
