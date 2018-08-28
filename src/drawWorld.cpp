
#include <SFML/Graphics.hpp>
#include "WorldState.hpp"

const sf::Vector2f tileSize = { 50.0f, 50.0f };

const sf::Color tileColors[(size_t)Tile::TileCount] = {
  sf::Color(0x303030FF),
  sf::Color(0x101010FF),
  sf::Color(0x0F0F2FFF),
  sf::Color(0x0000FFFF)
};

void drawWorld(sf::RenderTarget& window, WorldState world) {
  float x0 = 0;
  float y0 = 0;

  sf::RectangleShape rectangle;
  rectangle.setSize(tileSize);

  Pos worldSize = world.tiles.getSize();
  for (long i = 0; i < worldSize.x; ++i) {
    for (long j = 0; j < worldSize.y; ++j) {
      const Tile tileId = world.tiles.getTile(i, j);
      rectangle.setFillColor(tileColors[(size_t)tileId]);
      rectangle.setPosition(x0 + i * tileSize.x, y0 + j * tileSize.y);
      window.draw(rectangle);
    }
  }

  sf::CircleShape circle;
  circle.setRadius(std::min(tileSize.x, tileSize.y) * 0.4f);
  circle.setPointCount(16);
  circle.setFillColor(sf::Color::Red);
  for (const GuardState& guard: world.guards) {
    circle.setPosition(
      x0 + guard.pos.x * tileSize.x,
      y0 + guard.pos.y * tileSize.y
    );
    window.draw(circle);
  }

  circle.setFillColor(sf::Color::Green);
  circle.setPosition(
    x0 + world.player.pos.x * tileSize.x,
    y0 + world.player.pos.y * tileSize.y
  );
  window.draw(circle);
}
