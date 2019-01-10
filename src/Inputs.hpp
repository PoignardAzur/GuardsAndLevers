
#ifndef H_INPUTS
#define H_INPUTS

#include <SFML/Graphics.hpp>
#include <array>
#include <random>
#include <string_view>

#include "Scene.hpp"

class Scene;

class Inputs {
public:
  Inputs(sf::Window* window, unsigned seed);

  bool isKeyPressed(const std::string_view action) const;
  sf::Vector2f cursorPosition() const;
  bool isWindowClosed() const;

  std::default_random_engine& rng();

  void update(Scene& scene);

private:
  sf::Window* _window;
  std::default_random_engine _rng;

  std::array<bool, sf::Keyboard::KeyCount> _keysPressed;
  std::array<bool, sf::Mouse::ButtonCount> _mouseButtonsPressed;
  sf::Vector2f _cursorPosition;
  bool _isWindowClosed = false;
};

#endif // !H_INPUTS
