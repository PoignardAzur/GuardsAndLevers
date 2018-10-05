
#include <cassert>
#include "AnimationState.hpp"

sf::Vector2f UnitAnimation::getDeltaPos() const {
  switch (this->moveType) {
    case MoveType::Move: {
      Pos dpos = getDeltaPosFromDir(this->dir);
      float progression = (float)msLifeTime / (float)msDuration - 1.0;

      return { progression * dpos.x, progression * dpos.y };
    }
    case MoveType::Bump:
    case MoveType::GetAngry: {
      Pos dpos = getDeltaPosFromDir(this->dir);
      float progression = (float)msLifeTime / (float)msDuration;

      if (progression > 0.5)
        progression = 1 - progression;

      return { progression * dpos.x, progression * dpos.y };
    }
    case MoveType::Idle:
      return { 0, 0 };
  }
  assert(0);
}

time_t AnimationState::msMaxDuration() const {
  time_t msMax = 0;
  for (const UnitAnimation& anim: this->unitAnimations) {
    msMax = std::max(msMax, anim.msDuration);
  }
  return msMax;
}

Pos getDeltaPosFromDir(Direction dir) {
  switch (dir) {
    case Direction::Up:
      return { 0, -1 };
    case Direction::Right:
      return { 1, 0 };
    case Direction::Down:
      return { 0, 1 };
    case Direction::Left:
      return { -1, 0 };
  }
  assert(0);
}
