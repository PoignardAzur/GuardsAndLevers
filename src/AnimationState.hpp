
#ifndef ANIMATION_STATE_HPP
#define ANIMATION_STATE_HPP

#include "UnitAnimation.hpp"

struct AnimationState {
  std::vector<UnitAnimation> unitAnimations;

  time_t msMaxDuration() const;
};

#endif // !ANIMATION_STATE_HPP
