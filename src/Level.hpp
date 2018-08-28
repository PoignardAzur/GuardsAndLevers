
#ifndef H_LEVEL
#define H_LEVEL

#include <SFML/Graphics.hpp>
#include <memory>
#include <deque>
#include <random>

#include "Scene.hpp"

class Inputs;

class Level : public Scene {
public:
  Level();

  // void onKeyFirstPressed(Inputs& inputs, sf::Keyboard::Key key);
  // void onKeyReleased(Inputs& inputs, sf::Keyboard::Key key);
  // void onMbFirstPressed(Inputs& inputs, sf::Mouse::Button b, sf::Vector2f cursor);
  // void onMbReleased(Inputs& inputs, sf::Mouse::Button b, sf::Vector2f cursor);

  void update(Inputs& inputs);
  void display(sf::RenderTarget& window) const;

  using Rng = std::default_random_engine;

  using float_dice = std::uniform_real_distribution<float>;
  using double_dice = std::uniform_real_distribution<double>;
  using int_dice= std::uniform_int_distribution<int>;

private:
};

#endif // !H_LEVEL
