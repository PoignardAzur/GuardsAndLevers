
#ifndef ANIMATION_STATE_HPP
#define ANIMATION_STATE_HPP

#include <vector>
#include <ctime>
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Color.hpp>
#include "Grid2.hpp"

enum class Direction {
  Up,
  Right,
  Down,
  Left
};

enum class MoveType {
  Move,
  Bump,
  Idle,
  GetAngry
};

struct UnitAnimation {
  sf::Vector2f getDeltaPos() const;

  MoveType moveType;
  Direction dir;
  sf::Color color;
  time_t msDuration;
  time_t msLifeTime = 0;
};

struct AnimationState {
  std::vector<UnitAnimation> unitAnimations;
  // Grid2<Tile> tiles;

  time_t msMaxDuration() const;
};

Pos getDeltaPosFromDir(Direction dir);

#endif // !ANIMATION_STATE_HPP
