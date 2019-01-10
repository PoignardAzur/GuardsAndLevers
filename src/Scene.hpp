
#ifndef H_SCENE
#define H_SCENE

#include <SFML/Graphics.hpp>
#include <string_view>

#include "Inputs.hpp"

class Inputs;

class Scene {
public:
  virtual void onKeyPressed(Inputs&, const std::string_view action, bool fp) {}
  virtual void onKeyFirstPressed(Inputs&, const std::string_view action) {}
  virtual void onKeyReleased(Inputs&, const std::string_view action) {}

  virtual void update(Inputs& inputs) = 0;
  virtual void display(sf::RenderTarget& window) const = 0;

  virtual ~Scene() {};
};

#endif // !H_SCENE
