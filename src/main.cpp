#include <iostream>
#include <chrono>
#include <SFML/Graphics.hpp>
#include <tmxlite/Map.hpp>

#include "Inputs.hpp"
#include "Level.hpp"

int main(int argc, char** argv) {
  tmx::Map levelData;

  if (argc != 2) {
    std::cerr << "Usage: " << argv[0] << " <filename.tmx>" << std::endl;
    return 1;
  }

  if (!levelData.load(argv[1])) {
    std::cerr << "Failed to load level data at " << argv[1] << std::endl;
    return 1;
  }

  sf::RenderWindow window(sf::VideoMode(1080, 800), "GuardsAndLevers");
  window.setFramerateLimit(60);

  unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
  Inputs inputs(&window, seed);
  Level level(levelData);

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
