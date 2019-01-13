
#include "Inputs.hpp"

Inputs::Inputs(sf::Window* window, nk_context *guiContext, unsigned seed) :
  _window(window),
  _guiContext(guiContext),
  _rng(seed)
{
  _keysPressed.fill(false);
  _mouseButtonsPressed.fill(false);
}


bool Inputs::isKeyPressed(sf::Keyboard::Key key) const {
  return _keysPressed[key];
}

bool Inputs::isMbPressed(sf::Mouse::Button button) const {
  return _mouseButtonsPressed[button];
}

sf::Vector2f Inputs::cursorPosition() const {
  return _cursorPosition;
}

bool Inputs::isWindowClosed () const {
  return _isWindowClosed;
}

std::default_random_engine& Inputs::rng() {
  return _rng;
}

void Inputs::update(Scene& scene) {
  sf::Event event;

  nk_input_begin(_guiContext);
  while (_window->pollEvent(event)) {
    nk_sfml_handle_event(&event);

    switch (event.type) {
      case sf::Event::Closed:
        _isWindowClosed = true;
      break;

      case sf::Event::KeyPressed:
        {
          auto key = event.key.code;
          bool firstTime = !_keysPressed[key];
          scene.onKeyPressed(*this, key, firstTime);
          if (firstTime)
            scene.onKeyFirstPressed(*this, key);
          _keysPressed[key] = true;
        }
      break;
      case sf::Event::KeyReleased:
        scene.onKeyReleased(*this, event.key.code);
        _keysPressed[event.key.code] = false;
      break;
      case sf::Event::MouseButtonPressed:
        {
          auto button = event.mouseButton.button;
          bool firstTime = !_mouseButtonsPressed[button];
          scene.onMbPressed(*this, button, _cursorPosition, firstTime);
          if (firstTime)
            scene.onMbFirstPressed(*this, button, _cursorPosition);
          _mouseButtonsPressed[button] = true;
        }
      break;
      case sf::Event::MouseButtonReleased:
        scene.onMbReleased(*this, event.mouseButton.button, _cursorPosition);
        _mouseButtonsPressed[event.mouseButton.button] = false;
      break;

      case sf::Event::MouseMoved:
        _cursorPosition = sf::Vector2f(event.mouseMove.x, event.mouseMove.y);
      break;

      default:
        // TODO
      break;
    }
  }
  nk_input_end(_guiContext);
}
