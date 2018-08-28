
#ifndef H_SCENE
#define H_SCENE

#include <SFML/Graphics.hpp>

#include "Inputs.hpp"

class Inputs;

class Scene {
public:
  virtual void onKeyPressed(Inputs&, sf::Keyboard::Key key, bool) {}
  virtual void onKeyFirstPressed(Inputs&, sf::Keyboard::Key key) {}
  virtual void onKeyReleased(Inputs&, sf::Keyboard::Key key) {}
  virtual void onMbPressed(Inputs&, sf::Mouse::Button, sf::Vector2f, bool) {}
  virtual void onMbFirstPressed(Inputs&, sf::Mouse::Button, sf::Vector2f) {}
  virtual void onMbReleased(Inputs&, sf::Mouse::Button, sf::Vector2f) {}

  virtual void update(Inputs& inputs) = 0;
  virtual void display(sf::RenderTarget& window) const = 0;

  virtual ~Scene() {};
};

#endif // !H_SCENE
