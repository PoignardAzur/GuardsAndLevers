
#include "UnitAnimation.hpp"

sf::Vector2f UnitAnimation::getDeltaPos(int animTime) const {
  switch (this->action.type) {
    case UnitAction::Type::MoveAction: {
      int maxTime = 1000;

      Pos dpos = getDeltaPos(this->action.dir);
      float progression = (float)animTime / (float)maxTime;

      return { progression * dpos.x, progression * dpos.y };
    }
    case UnitAction::Type::BumpAction: {
      int maxTime = 1000;

      Pos dpos = getDeltaPos(this->action.dir);
      float progression = (float)animTime / (float)maxTime;

      if (progression > 0.5)
        progression = 0.5 - progression;

      return { progression * dpos.x, progression * dpos.y };
    }
    case UnitAction::Type::IdleAction:
      return { 0, 0 };
  }
}
