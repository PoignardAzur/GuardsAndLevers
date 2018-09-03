
#ifndef UNIT_ACTION_HPP
#define UNIT_ACTION_HPP

#include "Grid2.hpp"

struct UnitAction {
  enum class Type {
    MoveAction,
    BumpAction,
    IdleAction
  };

  enum class Direction {
    Up,
    Right,
    Down,
    Left
  };

  Type type;
  Direction dir;
};

Pos getDeltaPos(Direction dir);

#endif // !UNIT_ACTION_HPP
