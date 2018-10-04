
#include <cassert>
#include "UnitAction.hpp"

void UnitAction::applyChanges(WorldState::Unit& unit) const {
  if (this->type == UnitAction::Type::MoveAction) {
    Pos dpos = getDeltaPosFromDir(this->dir);
    unit.pos->x += dpos.x;
    unit.pos->y += dpos.y;
  }
}
