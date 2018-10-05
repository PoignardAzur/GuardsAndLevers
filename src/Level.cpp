
#include <cmath>
#include <algorithm>
#include <string>

#include "Level.hpp"
#include "units.hpp"

Level::Level() {
  m_world = {
    Grid2<Tile>({10, 10}, Tile::Wall),
    { GuardState {{{2, 6}, Direction::Up}, { {7, 7}, {2, 7} }} },
    PlayerState {{{6, 3}, Direction::Up}}
  };

  for (long x = 1; x < 9; ++x) {
    for (long y = 1; y < 9; ++y)
      m_world.tiles.set(x, y, Tile::Ground);
  }
  m_world.tiles.set(2, 2, Tile::OpenDoor);
  m_world.tiles.set(3, 2, Tile::ClosedDoor);

  m_units = m_world.getUnits();
  m_animations = ActionState(m_world.guards.size()).makeAnimationState();

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
    case sf::Keyboard::S:
    case sf::Keyboard::Up:
    case sf::Keyboard::Right:
    case sf::Keyboard::Down:
    case sf::Keyboard::Left: {
      if (!waitingForAnimations()) {
        onPlayerMove(getPlayerAction(key));
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

    animation.msLifeTime += dt;
    if (animation.msLifeTime >= animation.msDuration) {
      // FIXME
      animation = {
        MoveType::Idle,
        Direction::Up,
        animation.color,
        animation.msDuration
      };
    }
  }

  if (m_msTimeUntilNext > dt) {
    m_msTimeUntilNext -= dt;
  }
  else {
    m_msTimeUntilNext = 0;

    if (!m_nextActions.empty()) {
      m_nextActions[0].applyChanges(m_world);
      updateLos();

      m_animations = m_nextActions[0].makeAnimationState();
      m_msTimeUntilNext = m_animations.msMaxDuration();
      m_nextActions.pop_front();
    }
  }
}

void Level::display(sf::RenderTarget& window) const {
  window.clear();
  drawWorld(window);
}

bool Level::waitingForAnimations() const {
  return m_msTimeUntilNext != 0 || !m_nextActions.empty();
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
