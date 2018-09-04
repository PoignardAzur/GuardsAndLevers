
#ifndef UNIT_ACTION_HPP
#define UNIT_ACTION_HPP

#include "WorldState.hpp"

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

  void applyChanges(WorldState::Unit& unit) const;

  Type type = Type::IdleAction;
  Direction dir = Direction::Up;
};

Pos getDeltaPosFromDir(UnitAction::Direction dir);

#endif // !UNIT_ACTION_HPP
