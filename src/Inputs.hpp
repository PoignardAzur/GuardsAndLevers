
#ifndef H_INPUTS
#define H_INPUTS

#include <nuklear_h.h>
#include <nuklear_sfml_gl3.h>
#include <SFML/Graphics.hpp>
#include <array>
#include <random>

#include "Scene.hpp"

class Scene;

class Inputs {
public:
  Inputs(sf::Window* window, nk_context *guiContext, unsigned seed);

  bool isKeyPressed(sf::Keyboard::Key key) const;
  bool isMbPressed(sf::Mouse::Button button) const;
  sf::Vector2f cursorPosition() const;
  bool isWindowClosed() const;

  std::default_random_engine& rng();

  void update(Scene& scene);

private:
  sf::Window* _window;
  nk_context *_guiContext;
  std::default_random_engine _rng;

  std::array<bool, sf::Keyboard::KeyCount> _keysPressed;
  std::array<bool, sf::Mouse::ButtonCount> _mouseButtonsPressed;
  sf::Vector2f _cursorPosition;
  bool _isWindowClosed = false;
};

#endif // !H_INPUTS
