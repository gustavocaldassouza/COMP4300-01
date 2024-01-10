#include <SFML/Graphics.hpp>
#include <dirent.h>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/System/Vector2.hpp>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

std::array<uint, 2> handleWindowProps(std::istringstream &iss)
{
  std::array<uint, 2> window;
  int index_line = 0;
  for (std::string word; iss >> word;)
  {
    window[index_line] = std::stoi(word);
    index_line++;
  }
  return window;
}

std::array<std::string, 5> handleFontProps(std::istringstream &iss)
{
  std::array<std::string, 5> window;
  int index_line = 0;
  for (std::string word; iss >> word;)
  {
    window[index_line] = word;
    index_line++;
  }
  return window;
}

std::array<std::string, 9> handleCircleProps(std::istringstream &iss)
{
  std::array<std::string, 9> window;
  std::size_t index = 0;
  for (std::string word; iss >> word;)
  {
    window[index] = word;
    index++;
  }
  return window;
}

std::array<std::string, 10> handleRectangleProps(std::istringstream &iss)
{
  std::array<std::string, 10> window;
  std::size_t index = 0;
  for (std::string word; iss >> word;)
  {
    window[index] = word;
    index++;
  }
  return window;
}

int main()
{
  std::vector<sf::Text> text_shapes;
  std::vector<std::array<float, 2>> text_shapes_speeds;
  std::vector<sf::CircleShape> circle_shapes;
  std::vector<std::array<float, 2>> circle_shapes_speeds;
  std::vector<sf::RectangleShape> rectangle_shapes;
  std::vector<std::array<float, 2>> rectangle_shapes_speeds;

  sf::Font font;
  sf::Color font_color;
  uint font_size;

  uint window_width, window_height = 0;
  std::ifstream file("../config.txt");

  if (file.is_open())
  {
    std::string line;
    while (std::getline(file, line))
    {
      std::istringstream iss(line);
      std::string word;
      iss >> word;
      if (word == "Window")
      {
        auto window_props = handleWindowProps(iss);
        window_width = window_props[0];
        window_height = window_props[1];
      }
      if (word == "Font")
      {
        auto font_props = handleFontProps(iss);
        std::string path = "../" + font_props[0];
        font.loadFromFile(path);
        font_size = std::stoi(font_props[1]);
        font_color = sf::Color(std::stoi(font_props[2]), std::stoi(font_props[3]), std::stoi(font_props[4]));
      }
      if (word == "Circle")
      {
        auto circle_props = handleCircleProps(iss);

        sf::CircleShape circle(std::stof(circle_props[8]));
        circle.setPosition(std::stof(circle_props[1]), std::stof(circle_props[2]));
        circle.setFillColor(
            sf::Color(std::stoi(circle_props[5]), std::stoi(circle_props[6]), std::stoi(circle_props[7])));
        circle_shapes_speeds.push_back({std::stof(circle_props[3]), std::stof(circle_props[4])});
        circle_shapes.push_back(circle);

        sf::Text text(circle_props[0], font, font_size);
        float x = std::stof(circle_props[1]); //+ (std::stof(rectangle_props[8]) / 2) - text.getCharacterSize();
        float y = std::stof(circle_props[2]); // + (std::stof(rectangle_props[9]) / 2) - text.getCharacterSize();
        text.setPosition(x, y);
        text.setFillColor(font_color);

        text_shapes_speeds.push_back({std::stof(circle_props[3]), std::stof(circle_props[4])});
        text_shapes.push_back(text);
      }
      if (word == "Rectangle")
      {
        auto rectangle_props = handleRectangleProps(iss);
        sf::RectangleShape rectangle(sf::Vector2f(std::stof(rectangle_props[8]), std::stof(rectangle_props[9])));
        rectangle.setPosition(std::stof(rectangle_props[1]), std::stof(rectangle_props[2]));
        rectangle.setFillColor(
            sf::Color(std::stoi(rectangle_props[5]), std::stoi(rectangle_props[6]), std::stoi(rectangle_props[7])));

        rectangle_shapes_speeds.push_back({std::stof(rectangle_props[3]), std::stof(rectangle_props[4])});
        rectangle_shapes.push_back(rectangle);

        sf::Text text(rectangle_props[0], font, font_size);
        float x = std::stof(rectangle_props[1]); //+ (std::stof(rectangle_props[8]) / 2) - text.getCharacterSize();
        float y = std::stof(rectangle_props[2]); // + (std::stof(rectangle_props[9]) / 2) - text.getCharacterSize();
        text.setPosition(x, y);
        text.setFillColor(font_color);

        text_shapes_speeds.push_back({std::stof(rectangle_props[3]), std::stof(rectangle_props[4])});
        text_shapes.push_back(text);
      }
    }
  }
  file.close();

  auto window =
      sf::RenderWindow{{window_width, window_height}, "First Assignment"};
  window.setFramerateLimit(60);

  while (window.isOpen())
  {
    for (auto event = sf::Event{}; window.pollEvent(event);)
    {
      if (event.type == sf::Event::Closed)
      {
        window.close();
      }
    }

    window.clear();

    std::size_t index = 0;
    for (auto &shape : circle_shapes)
    {
      window.draw(shape);
      shape.move(circle_shapes_speeds[index][0], circle_shapes_speeds[index][1]);
      index++;
    }
    index = 0;
    for (auto &shape : rectangle_shapes)
    {
      window.draw(shape);
      shape.move(rectangle_shapes_speeds[index][0], rectangle_shapes_speeds[index][1]);
      index++;
    }
    index = 0;
    for (auto &shape : text_shapes)
    {
      window.draw(shape);
      shape.move(text_shapes_speeds[index][0], text_shapes_speeds[index][1]);
      index++;
    }

    window.display();
  }
}
