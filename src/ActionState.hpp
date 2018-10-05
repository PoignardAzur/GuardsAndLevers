
#ifndef ACTION_STATE_HPP
#define ACTION_STATE_HPP

#include <vector>
#include "AnimationState.hpp"

struct PlayerState;
struct GuardState;
struct WorldState;

class UnitAction {
public:
  // TODO - idle anims have random duration

  MoveType type;
  Direction dir;
};

class PlayerAction : public UnitAction {
public:
  void applyChanges(PlayerState& player) const;
  UnitAnimation getAnimation() const;
};

class GuardAction : public UnitAction {
public:
  void applyChanges(GuardState& guard) const;
  UnitAnimation getAnimation() const;
};


struct ActionState {
  explicit ActionState(size_t guardCount);

  std::vector<UnitAction*> getAllActions();
  AnimationState makeAnimationState() const;
  void applyChanges(WorldState& world) const;
  // std::vector<UnitAction*> getUnitActions();

  PlayerAction playerAction;
  std::vector<GuardAction> guardActions;
};

#endif // !ACTION_STATE_HPP
