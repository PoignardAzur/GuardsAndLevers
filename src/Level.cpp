
#include <cmath>
#include <algorithm>
#include <string>

#include "Level.hpp"
#include "units.hpp"

Level::Level(nk_context *guiContext) {
  m_guiContext = guiContext;

  WorldState world = {
    Grid2<Tile>({10, 10}, Tile::Wall),
    {
      GuardState {{{2, 6}, Direction::Up}, { {7, 7}, {2, 7} }}
    },
    PlayerState {{{6, 3}, Direction::Up}}
  };

  for (long x = 1; x < 9; ++x) {
    for (long y = 1; y < 9; ++y)
      world.tiles.set(x, y, Tile::Ground);
  }
  world.tiles.set(2, 2, Tile::OpenDoor);
  world.tiles.set(3, 2, Tile::ClosedDoor);

  m_levelLogic.emplace(world);

  m_animations = ActionState(m_levelLogic->guardCount).makeAnimationState();
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
        m_nextActions = m_levelLogic->onPlayerMove(getPlayerAction(key));
        m_msTimeUntilNext = 0;
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

  for (UnitAnimation& animation : m_animations.unitAnimations) {
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
      m_nextActions[0].applyChanges(m_levelLogic->world);
      m_levelLogic->updateLos();

      m_animations = m_nextActions[0].makeAnimationState();
      m_msTimeUntilNext = m_animations.msMaxDuration();
      m_nextActions.pop_front();
    }
  }
}

void Level::display(sf::RenderTarget& window) const {
  window.clear();
  drawWorld(window);

  auto winSize = window.getSize();
  if (nk_begin(
    m_guiContext, "Demo",
    nk_rect(winSize.x / 2 - 50, winSize.y - 150, 100, 100),
    NK_WINDOW_TITLE | NK_WINDOW_MOVABLE
  ))
  {}
  nk_end(m_guiContext);
}

PlayerAction Level::getPlayerAction(sf::Keyboard::Key key) const {
  Direction dir;

  switch (key) {
    case sf::Keyboard::S: {
      return {{ MoveType::Bump, Direction::Up }};
    }
    case sf::Keyboard::Up: {
      dir = Direction::Up;
      break;
    }
    case sf::Keyboard::Right: {
      dir = Direction::Right;
      break;
    }
    case sf::Keyboard::Down: {
      dir = Direction::Down;
      break;
    }
    case sf::Keyboard::Left: {
      dir = Direction::Left;
      break;
    }
    default: {
      assert(0);
    }
  }
  return {{ MoveType::Move, dir }};
}

bool Level::waitingForAnimations() const {
  return m_msTimeUntilNext != 0 || !m_nextActions.empty();
}
