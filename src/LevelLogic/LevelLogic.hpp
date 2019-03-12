
#ifndef LEVEL_LOGIC_HPP
#define LEVEL_LOGIC_HPP

#include <deque>
#include <unordered_map>

#include "WorldState.hpp"
#include "ActionState.hpp"
#include "AnimationState.hpp"

namespace std {
  template <>
  struct hash<Pos>
  {
    std::size_t operator()(const Pos& pos) const
    {
      return std::hash<long>()(pos.x) ^ std::hash<long>()(pos.y) << 1;
    }
  };
}

struct LevelLogic {
  LevelLogic(WorldState state);

  std::deque<ActionState> onPlayerMove(PlayerAction playerAction);
  GuardAction nextGuardMovement(const Grid2<int>& distancesToPlayer, size_t i);

  void updateLos();
  void updatePathfinding();

  WorldState world;

  std::vector<UnitState*> units;
  size_t guardCount = 0;

  std::vector<Grid2<char>> individualLosTokens;
  Grid2<char> collectiveLosTokens;
  std::unordered_map<Pos, Grid2<int>> pathfindings;
};

#endif // !LEVEL_LOGIC_HPP
