#include <SFML/Graphics.hpp>

int main() {
  auto window = sf::RenderWindow{{1280u, 720u}, "CMake SFML Project"};
  window.setFramerateLimit(144);
  sf::CircleShape shape(100.f);
  shape.setFillColor(sf::Color::Magenta);

  while (window.isOpen()) {
    for (auto event = sf::Event{}; window.pollEvent(event);) {
      if (event.type == sf::Event::Closed) {
        window.close();
      }
    }

    window.clear();
    window.draw(shape);
    window.display();
  }
}
