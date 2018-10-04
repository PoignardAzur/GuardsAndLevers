
#ifndef UNIT_ACTION_HPP
#define UNIT_ACTION_HPP

#include "WorldState.hpp"

struct UnitAction {
  enum class Type {
    MoveAction,
    BumpAction,
    IdleAction
  };

  void applyChanges(WorldState::Unit& unit) const;

  Type type = Type::IdleAction;
  Direction dir = Direction::Up;
};

#endif // !UNIT_ACTION_HPP
