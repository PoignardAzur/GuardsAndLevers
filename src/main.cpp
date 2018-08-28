#include <chrono>
#include <SFML/Graphics.hpp>

#include "Inputs.hpp"
#include "Level.hpp"

int main() {
  sf::RenderWindow window(sf::VideoMode(1080, 800), "GuardsAndLevers");
  window.setFramerateLimit(60);

  unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
  Inputs inputs(&window, seed);
  Level level;

  while (window.isOpen()) {
    inputs.update(level);

    if (inputs.isWindowClosed()) {
      window.close();
    }
    else {
      level.update(inputs);
      level.display(window);
      window.display();
    }
  }

  return 0;
}
