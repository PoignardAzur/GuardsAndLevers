
#include <cmath>
#include <algorithm>
#include <string>

#include "Level.hpp"
#include "units.hpp"

#include <iostream>

Level::Level() {
  m_world = {
    Grid2<Tile>({10, 10}, Tile::Ground),
    { GuardState {2, 3} },
    PlayerState {3, 3}
  };

  for (long i = 0; i < 10; ++i) {
    m_world.tiles.set(i, 0, Tile::Wall);
    m_world.tiles.set(i, 9, Tile::Wall);
  }
  for (long j = 0; j < 10; ++j) {
    m_world.tiles.set(0, j, Tile::Wall);
    m_world.tiles.set(9, j, Tile::Wall);
  }

  m_world.tiles.set(2, 2, Tile::OpenDoor);
  m_world.tiles.set(3, 2, Tile::ClosedDoor);

  m_units = m_world.getUnits();
  m_animations.unitAnimations.resize(m_units.size(), {{
    UnitAction::Type::MoveAction,
    UnitAction::Direction::Up
  }});
}

void Level::update(Inputs& inputs) {
  // FIXME
  m_msAnimTime += 5;
}

void Level::display(sf::RenderTarget& window) const {
  window.clear();
  drawWorld(window);
}

void Level::setAnimations(AnimationState animations) {
  m_animations = std::move(animations);
  m_msAnimTime = 0;
  m_msAnimMaxTime = m_animations.msMaxDuration();
}
