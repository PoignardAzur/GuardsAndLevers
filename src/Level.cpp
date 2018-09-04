
#include <cmath>
#include <algorithm>
#include <string>

#include "Level.hpp"
#include "units.hpp"

#include <iostream>

Level::Level() {
  m_world = {
    Grid2<Tile>({10, 10}, Tile::Wall),
    { GuardState {2, 3} },
    PlayerState {3, 3}
  };

  for (long x = 1; x < 9; ++x) {
    for (long y = 1; y < 9; ++y)
      m_world.tiles.set(x, y, Tile::Ground);
  }
  m_world.tiles.set(2, 2, Tile::OpenDoor);
  m_world.tiles.set(3, 2, Tile::ClosedDoor);

  m_units = m_world.getUnits();
  m_animations.unitAnimations.resize(m_units.size(), {});
}

void Level::update(Inputs& inputs) {
  // FIXME
  time_t dt = 50;

  for (size_t i = 0; i < m_units.size(); ++i) {
    UnitAnimation& animation = m_animations.unitAnimations[i];
    WorldState::Unit& unit = m_units[i];

    animation.msLifeTime += dt;
    if (animation.msLifeTime >= animation.msDuration()) {
      animation.action.applyChanges(unit);
      animation = {};
    }
  }

  if (m_msTimeUntilNext > dt) {
    m_msTimeUntilNext -= dt;
  }
  else {
    m_msTimeUntilNext = 0;

    if (!m_nextAnimations.empty()) {
      for (size_t i = 0; i < m_units.size(); ++i) {
        auto& currentAnim = m_animations.unitAnimations[i];
        const auto& nextAnim = m_nextAnimations[0].unitAnimations[i];

        if (nextAnim.action.type != UnitAction::Type::IdleAction) {
          currentAnim = nextAnim;
        }
      }
      m_nextAnimations.pop_front();
    }
  }
}

void Level::display(sf::RenderTarget& window) const {
  window.clear();
  drawWorld(window);
}

bool Level::waitingForAnimations() const {
  return m_msTimeUntilNext != 0 || !m_nextAnimations.empty();
}
