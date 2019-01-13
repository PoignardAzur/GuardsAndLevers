#include <chrono>
#include <cstdio>
#include <SFML/Graphics.hpp>

#include "Inputs.hpp"
#include "Level.hpp"

#include <nuklear_h.h>
#include <nuklear_sfml_gl3.h>

#define MAX_VERTEX_BUFFER 512 * 1024
#define MAX_ELEMENT_BUFFER 128 * 1024

int main() {
  sf::ContextSettings settings;
  settings.depthBits = 24;
  settings.stencilBits = 8;
  settings.antialiasingLevel = 4;
  settings.majorVersion = 3;
  settings.minorVersion = 0;
  sf::RenderWindow window(
    sf::VideoMode(1080, 800), "GuardsAndLevers", sf::Style::Default
  );
  window.setFramerateLimit(60);

  if (!gladLoadGL()) {
    printf("Failed to load OpenGL extensions!\n");
    return 1;
  }
  glViewport(0, 0, window.getSize().x, window.getSize().y);

  // GUI
  nk_context *ctx = nk_sfml_init(&window);
  nk_font_atlas *atlas;
  nk_sfml_font_stash_begin(&atlas);
  nk_font *fontClean = nk_font_atlas_add_from_file(
    atlas, "assets/ProggyClean.ttf", 12, 0
  );
  nk_sfml_font_stash_end();
  nk_style_set_font(ctx, &fontClean->handle);

  unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
  Inputs inputs(&window, ctx, seed);
  Level level(ctx);

  while (window.isOpen()) {
    inputs.update(level);

    if (inputs.isWindowClosed()) {
      window.close();
    }
    else {
      level.update(inputs);

      window.setActive(true);
      level.display(window);
      nk_sfml_render(NK_ANTI_ALIASING_ON, MAX_VERTEX_BUFFER, MAX_ELEMENT_BUFFER);
      window.display();
    }
  }

  nk_sfml_shutdown();
  return 0;
}
