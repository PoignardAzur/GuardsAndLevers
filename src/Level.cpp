
#include <cassert>
#include <cmath>
#include <algorithm>
#include <string>

#include <tmxlite/Layer.hpp>
#include <tmxlite/TileLayer.hpp>
#include <tmxlite/ObjectGroup.hpp>

#include "Level.hpp"
#include "units.hpp"

static WorldState _loadLevel(const tmx::Map& levelData) {
  const auto& layers = levelData.getLayers();

  assert(layers.size() == 3);
  assert(layers[0]->getType() == tmx::Layer::Type::Tile);
  assert(layers[1]->getType() == tmx::Layer::Type::Object);
  assert(layers[2]->getType() == tmx::Layer::Type::Object);
  assert(layers[1]->getName() == "Enemies");
  assert(layers[2]->getName() == "Player");

  const auto& tiles = layers[0]->getLayerAs<tmx::TileLayer>().getTiles();
  const auto& enemyGroup = layers[1]->getLayerAs<tmx::ObjectGroup>();
  const auto& playerGroup = layers[2]->getLayerAs<tmx::ObjectGroup>();
  const auto tileCount = levelData.getTileCount();
  const auto tileSize = levelData.getTileSize();

  // TILES
  Grid2 tileGrid({ tileCount.x, tileCount.y }, Tile::Ground);
  for (size_t i = 0; i < tiles.size(); ++i) {
    size_t x = i % tileCount.x;
    size_t y = i / tileCount.x;

    Tile tileValue;
    switch (tiles[i].ID) {
      case 19:
        tileValue = Tile::Wall;
        break;
      case 2:
        tileValue = Tile::OpenDoor;
        break;
      case 3:
        tileValue = Tile::ClosedDoor;
        break;
      default:
        tileValue = Tile::Ground;
        break;
    }

    tileGrid.set({x, y}, tileValue);
  }

  // ENEMIES
  std::vector<GuardState> guards;
  guards.reserve(enemyGroup.getObjects().size());
  const std::vector<Pos> patrolStops = { {7, 7}, {2, 7} };
  for (const auto& enemy: enemyGroup.getObjects()) {
    auto enemyPos = enemy.getPosition();
    GuardState guard = {
      {
        { (long)(enemyPos.x / tileSize.x), (long)(enemyPos.y / tileSize.y) },
        Direction::Up,
      },
      patrolStops
    };
    guards.push_back(guard);
  }

  // PLAYER
  assert(playerGroup.getObjects().size() == 1);
  auto playerPos = playerGroup.getObjects()[0].getPosition();
  PlayerState playerState = {{
    { (long)(playerPos.x / tileSize.x), (long)(playerPos.y / tileSize.y) },
    Direction::Up
  }};

  return {
    std::move(tileGrid),
    std::move(guards),
    playerState
  };
}

Level::Level(const tmx::Map& levelData) {
  m_levelLogic.emplace(_loadLevel(levelData));
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

  for (UnitAnimation& animation : m_levelLogic->unitAnimations) {
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

      m_levelLogic->unitAnimations = m_nextActions[0].makeAnimations(m_msTimeUntilNext);
      m_nextActions.pop_front();
    }
  }
}

void Level::display(sf::RenderTarget& window) const {
  window.clear();
  drawWorld(window);
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
