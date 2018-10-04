
#ifndef H_LEVEL
#define H_LEVEL

#include <SFML/Graphics.hpp>
#include <memory>
#include <deque>
#include <random>
#include <unordered_map>

#include "Scene.hpp"
#include "WorldState.hpp"
#include "AnimationState.hpp"

class Inputs;

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

class Level : public Scene {
public:
  Level();

  void onKeyFirstPressed(Inputs& inputs, sf::Keyboard::Key key);
  // void onKeyReleased(Inputs& inputs, sf::Keyboard::Key key);
  // void onMbFirstPressed(Inputs& inputs, sf::Mouse::Button b, sf::Vector2f cursor);
  // void onMbReleased(Inputs& inputs, sf::Mouse::Button b, sf::Vector2f cursor);

  void update(Inputs& inputs);
  void display(sf::RenderTarget& window) const;

  using Rng = std::default_random_engine;

  using float_dice = std::uniform_real_distribution<float>;
  using double_dice = std::uniform_real_distribution<double>;
  using int_dice = std::uniform_int_distribution<int>;

private:
  void onPlayerMove(sf::Keyboard::Key key);
  UnitAction nextGuardMovement(const Grid2<int>& distancesToPlayer, size_t i);
  bool waitingForAnimations() const;

  void updateLos();

  void drawWorld(sf::RenderTarget& window) const;

  WorldState m_world;
  AnimationState m_animations;

  std::vector<WorldState::Unit> m_units;

  std::vector<Grid2<char>> m_individualLosTokens;
  Grid2<char> m_collectiveLosTokens;
  std::unordered_map<Pos, Grid2<int>> m_pathfindings;

  std::deque<AnimationState> m_nextAnimations;
  time_t m_msTimeUntilNext = 0;
};

#endif // !H_LEVEL
