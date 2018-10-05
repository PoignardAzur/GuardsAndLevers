
#include <cassert>
#include "Level.hpp"

const sf::Vector2f tileSize = { 50.0f, 50.0f };

const sf::Color tileColors[(size_t)Tile::TileCount] = {
  sf::Color(0x303030FF),
  sf::Color(0x101010FF),
  sf::Color(0x0F0F2FFF),
  sf::Color(0x0000FFFF)
};

void Level::drawWorld(sf::RenderTarget& window) const {
  float x0 = 0;
  float y0 = 0;

  sf::RectangleShape rectangle;
  rectangle.setSize(tileSize);

  Pos worldSize = m_world.tiles.getSize();
  for (long x = 0; x < worldSize.x; ++x) {
    for (long y = 0; y < worldSize.y; ++y) {
      sf::Color losColor = m_collectiveLosTokens.get(x, y)
        ? sf::Color(0xFFFFFFFF)
        : sf::Color(0xE0E0E0FF);
      Tile tileId = m_world.tiles.get(x, y);
      rectangle.setFillColor(tileColors[(size_t)tileId] * losColor);
      rectangle.setPosition(x0 + x * tileSize.x, y0 + y * tileSize.y);
      window.draw(rectangle);
    }
  }

  // ---
  // DRAW PLAYER AND GUARDS
  // ---

  sf::CircleShape circle;
  circle.setRadius(std::min(tileSize.x, tileSize.y) * 0.4f);
  circle.setPointCount(16);

  assert(m_units.size() == m_animations.unitAnimations.size());
  for (size_t i = 0; i < m_units.size(); ++i) {
    const UnitState* unit = m_units[i];
    const UnitAnimation& animation = m_animations.unitAnimations[i];

    assert(animation.msLifeTime < animation.msDuration);
    // time_t msAnimTime = std::min(animation.msLifeTime, animation.msDuration);
    // spriteAnim = spriteSheet[unit.spriteName, animation.type, msAnimTime];
    circle.setFillColor(animation.color);

    sf::Vector2f dpos = animation.getDeltaPos();
    circle.setPosition(
      x0 + (unit->pos.x + dpos.x) * tileSize.x,
      y0 + (unit->pos.y + dpos.y) * tileSize.y
    );
    window.draw(circle);
  }
}
