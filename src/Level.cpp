
#include <cmath>
#include <algorithm>
#include <string>

#include "Level.hpp"
#include "units.hpp"

Level::Level() {
  m_world = {
    Grid2<Tile>({10, 10}, Tile::Wall),
    { GuardState {{2, 3}, Direction::Up, { {7, 7}, {2, 7} }} },
    PlayerState {{3, 3}}
  };

  for (long x = 1; x < 9; ++x) {
    for (long y = 1; y < 9; ++y)
      m_world.tiles.set(x, y, Tile::Ground);
  }
  m_world.tiles.set(2, 2, Tile::OpenDoor);
  m_world.tiles.set(3, 2, Tile::ClosedDoor);

  m_units = m_world.getUnits();
  m_animations.unitAnimations.resize(m_units.size(), {});

  m_individualLosTokens.resize(m_world.guards.size());
  updateLos();

  for (const GuardState& guard : m_world.guards) {
    for (Pos patrolStop : guard.patrolStops) {
      if (m_pathfindings.count(patrolStop) == 0) {
        m_pathfindings[patrolStop] = WorldState::getDistances(
          m_world.tiles, patrolStop
        );
      }
    }
  }

}

void Level::onKeyFirstPressed(Inputs& inputs, sf::Keyboard::Key key) {
  (void)inputs;

  switch (key) {
    case sf::Keyboard::Up:
    case sf::Keyboard::Right:
    case sf::Keyboard::Down:
    case sf::Keyboard::Left: {
      if (!waitingForAnimations()) {
        onPlayerMove(key);
      }
      break;
    }
    default:
    break;
  }
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
      m_msTimeUntilNext = m_nextAnimations[0].msMaxDuration();
      m_nextAnimations.pop_front();
    }
    else {
      updateLos();
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

void Level::updateLos() {
  m_collectiveLosTokens = Grid2<char>(m_world.tiles.getSize(), 0);

  for (size_t i = 0; i < m_world.guards.size(); ++i) {
    m_individualLosTokens[i] = WorldState::generateLosTokens(
      m_world.tiles, m_world.guards[i]
    );

    for (long x = 0; x < m_world.tiles.getSize().x; ++x) {
      for (long y = 0; y < m_world.tiles.getSize().y; ++y) {
        m_collectiveLosTokens.set(
          x, y,
          m_collectiveLosTokens.get(x, y)
          || m_individualLosTokens[i].get(x, y)
        );
      }
    }
  }
}
