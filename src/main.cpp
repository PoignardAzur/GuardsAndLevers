#include <chrono>
#include <SFML/Graphics.hpp>

#include "Inputs.hpp"
#include "Level.hpp"

#include <imgui/imgui.h>
#include <imgui/imgui-SFML.h>

int main() {
  sf::RenderWindow window(sf::VideoMode(1080, 800), "GuardsAndLevers");
  window.setFramerateLimit(60);

  unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
  Inputs inputs(&window, seed);
  Level level;

  ImGui::SFML::Init(window);
  sf::Clock deltaClock;

  while (window.isOpen()) {
    inputs.update(level);
    ImGui::SFML::Update(window, deltaClock.restart());

    if (inputs.isWindowClosed()) {
      window.close();
    }
    else {
      level.update(inputs);
      level.display(window);
      window.display();
    }
  }

  ImGui::SFML::Shutdown();
  return 0;
}
