
#include "AnimationState.hpp"

time_t AnimationState::msMaxDuration() const {
  time_t msMax = 0;

  for (const UnitAnimation& anim: this->unitAnimations) {
    msMax = std::max(msMax, anim.msDuration());
  }
  return msMax;
}
