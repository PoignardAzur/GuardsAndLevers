
#ifndef H_LEVEL
#define H_LEVEL

#include <tmxlite/Map.hpp>
#include <SFML/Graphics.hpp>
#include <memory>
#include <random>
#include <optional>

#include "LevelLogic/LevelLogic.hpp"
#include "Scene.hpp"

class Inputs;

class Level : public Scene {
public:
  Level(const tmx::Map& levelData);

  void onKeyFirstPressed(Inputs& inputs, sf::Keyboard::Key key);
  // void onKeyReleased(Inputs& inputs, sf::Keyboard::Key key);
  // void onMbFirstPressed(Inputs& inputs, sf::Mouse::Button b, sf::Vector2f cursor);
  // void onMbReleased(Inputs& inputs, sf::Mouse::Button b, sf::Vector2f cursor);

  void update(Inputs& inputs);
  void display(sf::RenderTarget& window) const;

private:
  PlayerAction getPlayerAction(sf::Keyboard::Key key) const;

  bool waitingForAnimations() const;
  void drawWorld(sf::RenderTarget& window) const;

  std::optional<LevelLogic> m_levelLogic;

  std::deque<ActionState> m_nextActions;
  time_t m_msTimeUntilNext = 0;
};

#endif // !H_LEVEL
