
#include <cassert>
#include "UnitAnimation.hpp"

sf::Vector2f UnitAnimation::getDeltaPos(time_t msAnimTime) const {
  switch (this->action.type) {
    case UnitAction::Type::MoveAction: {
      Pos dpos = getDeltaPosFromDir(this->action.dir);
      float progression = (float)msAnimTime / (float)msDuration();

      return { progression * dpos.x, progression * dpos.y };
    }
    case UnitAction::Type::BumpAction: {
      Pos dpos = getDeltaPosFromDir(this->action.dir);
      float progression = (float)msAnimTime / (float)msDuration();

      if (progression > 0.5)
        progression = 1 - progression;

      return { progression * dpos.x, progression * dpos.y };
    }
    case UnitAction::Type::IdleAction:
      return { 0, 0 };
  }
  assert(0);
}

time_t UnitAnimation::msDuration() const {
  return 400;
}
