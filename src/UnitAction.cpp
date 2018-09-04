
#include <cassert>
#include "UnitAction.hpp"

Pos getDeltaPosFromDir(UnitAction::Direction dir) {
  switch (dir) {
    case UnitAction::Direction::Up:
      return { 0, -1 };
    case UnitAction::Direction::Right:
      return { 1, 0 };
    case UnitAction::Direction::Down:
      return { 0, 1 };
    case UnitAction::Direction::Left:
      return { -1, 0 };
  }
  assert(0);
}
