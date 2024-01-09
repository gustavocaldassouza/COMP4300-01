#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/System/Vector2.hpp>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>

void read_file();

int main() {
  read_file();
  auto window = sf::RenderWindow{{1280u, 720u}, "CMake SFML Project"};

  window.setFramerateLimit(60);
  sf::RectangleShape rect(sf::Vector2f(30.f, 30.f));
  rect.setFillColor(sf::Color::Yellow);

  sf::CircleShape circle(100.f);
  circle.setFillColor(sf::Color::Magenta);

  float speed = 5.0f;
  float rect_speed = 5.0f;
  while (window.isOpen()) {
    for (auto event = sf::Event{}; window.pollEvent(event);) {
      if (event.type == sf::Event::Closed) {
        window.close();
      }
    }

    circle.move(speed, speed);
    rect.move(0, rect_speed);
    window.clear();
    if (circle.getPosition().y + 200 == 720 || circle.getPosition().y == 0) {
      speed = -speed;
      // std::cout << "Collision" << std::endl;
    }
    if (rect.getPosition().y + 30 == 720 || rect.getPosition().y == 0) {
      rect_speed = -rect_speed;
      // std::cout << "Collision" << std::endl;
    }

    // rect.move(speed, speed);
    window.draw(rect);
    window.draw(circle);
    window.display();
  }
}

void show_files() {
  std::string path = ".";
  for (const auto &entry : std::filesystem::directory_iterator(path))
    std::cout << entry.path() << std::endl;
}

void read_file() {
  show_files();
  std::string text;
  std::ifstream MyFile("./src/config.txt");
  if (MyFile.is_open()) {
    std::cout << "File opened" << std::endl;
    while (getline(MyFile, text)) {
      std::cout << text << std::endl;
    }
    MyFile.close();
  }
}
