
#include <cassert>
#include "WorldState.hpp"
#include "ActionState.hpp"

void PlayerAction::applyChanges(PlayerState& player) const {
  if (this->type == MoveType::Move) {
    Pos dpos = getDeltaPosFromDir(this->dir);
    player.pos.x += dpos.x;
    player.pos.y += dpos.y;
  }
  if (this->type == MoveType::Move || this->type == MoveType::Bump) {
    player.dir = this->dir;
  }
}

UnitAnimation PlayerAction::getAnimation() const {
  return {
    this->type,
    this->dir,
    sf::Color::Green,
    400
  };
}

void GuardAction::applyChanges(GuardState& guard) const {
  if (this->type == MoveType::Move) {
    Pos dpos = getDeltaPosFromDir(this->dir);
    guard.pos.x += dpos.x;
    guard.pos.y += dpos.y;
  }
  if (this->type == MoveType::Move || this->type == MoveType::Bump) {
    guard.dir = this->dir;
  }
  if (this->type == MoveType::GetAngry) {
    guard.isAngry = true;
  }
}

UnitAnimation GuardAction::getAnimation() const {
  return {
    this->type,
    this->dir,
    sf::Color::Red,
    400
  };
}


ActionState::ActionState(size_t guardCount) {
  playerAction = {};
  guardActions.resize(guardCount);
}

std::vector<UnitAction*> ActionState::getAllActions() {
  std::vector<UnitAction*> actions;

  actions.reserve(1 + this->guardActions.size());
  actions.push_back(&this->playerAction);
  for (GuardAction& action: this->guardActions) {
    actions.push_back(&action);
  }

  return actions;
}

AnimationState ActionState::makeAnimationState() const {
  AnimationState anims;

  anims.unitAnimations.reserve(1 + this->guardActions.size());
  anims.unitAnimations.push_back(this->playerAction.getAnimation());
  for (const GuardAction& action: this->guardActions) {
    anims.unitAnimations.push_back(action.getAnimation());
  }

  return anims;
}

void ActionState::applyChanges(WorldState& world) const {
  this->playerAction.applyChanges(world.player);
  for (size_t i = 0; i < this->guardActions.size(); ++i) {
    this->guardActions[i].applyChanges(world.guards[i]);
  }
}

/*
TODO - Don't interrupt idle anims

      for (size_t i = 0; i < m_units.size(); ++i) {
        auto& currentAnim = m_animations.unitAnimations[i];
        const auto& nextAnim = m_nextAnimations[0].unitAnimations[i];

        if (nextAnim.action.type != MoveType::Idle) {
          currentAnim = nextAnim;
        }
      }

*/
