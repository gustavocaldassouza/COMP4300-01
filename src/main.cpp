#include <SFML/Graphics.hpp>
#include <dirent.h>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/System/Vector2.hpp>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

void handleWindowProps(std::istringstream &iss, uint &windowWidth, uint &windowHeight)
{
  std::array<uint, 2> windowProps;
  int index_line = 0;

  for (std::string word; iss >> word;)
  {
    windowProps[index_line] = std::stoi(word);
    index_line++;
  }

  windowWidth = windowProps[0];
  windowHeight = windowProps[1];
}

void handleFontProps(std::istringstream &iss, sf::Font &font, uint &font_size, sf::Color &font_color)
{
  std::array<std::string, 5> fontProps;
  int index_line = 0;
  for (std::string word; iss >> word;)
  {
    fontProps[index_line] = word;
    index_line++;
  }

  std::string path = "../" + fontProps[0];
  font.loadFromFile(path);
  font_size = std::stoi(fontProps[1]);
  font_color = sf::Color(std::stoi(fontProps[2]), std::stoi(fontProps[3]), std::stoi(fontProps[4]));
}

void handleCircleProps(std::istringstream &iss, sf::Font &font, uint &font_size, sf::Color &font_color, std::vector<sf::CircleShape> &circleShapes, std::vector<std::array<float, 2>> &circleShapesSpeeds, std::vector<sf::Text> &textShapes, std::vector<std::array<float, 2>> &textShapesSpeeds)
{
  std::array<std::string, 9> circleProps;
  std::size_t index = 0;
  for (std::string word; iss >> word;)
  {
    circleProps[index] = word;
    index++;
  }

  sf::CircleShape circle(std::stof(circleProps[8]));
  circle.setPosition(std::stof(circleProps[1]), std::stof(circleProps[2]));
  circle.setFillColor(
      sf::Color(std::stoi(circleProps[5]), std::stoi(circleProps[6]), std::stoi(circleProps[7])));

  circleShapesSpeeds.push_back({std::stof(circleProps[3]), std::stof(circleProps[4])});
  circleShapes.push_back(circle);

  sf::Text text(circleProps[0], font, font_size);
  sf::FloatRect text_bounds = text.getLocalBounds();
  float x = std::stof(circleProps[1]) + (std::stof(circleProps[8])) - text_bounds.width / 2;
  float y = std::stof(circleProps[2]) + (std::stof(circleProps[8])) - text_bounds.height;
  text.setPosition(x, y);
  text.setFillColor(font_color);

  textShapesSpeeds.push_back({std::stof(circleProps[3]), std::stof(circleProps[4])});
  textShapes.push_back(text);
}

void handleRectangleProps(std::istringstream &iss, sf::Font &font, uint &font_size, sf::Color &font_color, std::vector<sf::RectangleShape> &rectangleShapes, std::vector<std::array<float, 2>> &rectangleShapesSpeeds, std::vector<sf::Text> &textShapes, std::vector<std::array<float, 2>> &textShapesSpeeds)
{
  std::array<std::string, 10> rectangleProps;
  std::size_t index = 0;
  for (std::string word; iss >> word;)
  {
    rectangleProps[index] = word;
    index++;
  }

  sf::RectangleShape rectangle(sf::Vector2f(std::stof(rectangleProps[8]), std::stof(rectangleProps[9])));
  rectangle.setPosition(std::stof(rectangleProps[1]), std::stof(rectangleProps[2]));
  rectangle.setFillColor(
      sf::Color(std::stoi(rectangleProps[5]), std::stoi(rectangleProps[6]), std::stoi(rectangleProps[7])));

  rectangleShapesSpeeds.push_back({std::stof(rectangleProps[3]), std::stof(rectangleProps[4])});
  rectangleShapes.push_back(rectangle);

  sf::Text text(rectangleProps[0], font, font_size);
  sf::FloatRect text_bounds = text.getLocalBounds();
  float x = std::stof(rectangleProps[1]) + (std::stof(rectangleProps[8]) / 2) - text_bounds.width / 2;
  float y = std::stof(rectangleProps[2]) + (std::stof(rectangleProps[9]) / 2) - text_bounds.height;
  text.setPosition(x, y);
  text.setFillColor(font_color);

  textShapesSpeeds.push_back({std::stof(rectangleProps[3]), std::stof(rectangleProps[4])});
  textShapes.push_back(text);
}

void drawCircles(std::vector<sf::CircleShape> &shapes, std::vector<std::array<float, 2>> &speeds, std::vector<std::array<float, 2>> &textSpeedShapes, sf::RenderWindow &window, std::size_t &indexTextArr)
{
  std::size_t index = 0;
  for (auto &shape : shapes)
  {
    shape.move(speeds[index][0], speeds[index][1]);

    if (shape.getPosition().x <= 0 || (shape.getPosition().x + (shape.getRadius() * 2)) >= window.getSize().x)
    {
      speeds[index][0] = -speeds[index][0];
      textSpeedShapes[index + indexTextArr][0] = -textSpeedShapes[index + indexTextArr][0];
    }
    if (shape.getPosition().y <= 0 || (shape.getPosition().y + (shape.getRadius() * 2)) > window.getSize().y)
    {
      speeds[index][1] = -speeds[index][1];
      textSpeedShapes[index + indexTextArr][1] = -textSpeedShapes[index + indexTextArr][1];
    }

    window.draw(shape);
    index++;
  }
}

void drawRectangles(std::vector<sf::RectangleShape> &shapes, std::vector<std::array<float, 2>> &speeds, std::vector<std::array<float, 2>> &textSpeedShapes, sf::RenderWindow &window, std::size_t &indexTextArr)
{
  std::size_t index = 0;
  for (auto &shape : shapes)
  {
    shape.move(speeds[index][0], speeds[index][1]);

    if (shape.getPosition().x <= 0 || (shape.getPosition().x + shape.getSize().x) >= window.getSize().x)
    {
      speeds[index][0] = -speeds[index][0];
      textSpeedShapes[index + indexTextArr][0] = -textSpeedShapes[index + indexTextArr][0];
    }
    if (shape.getPosition().y <= 0 || (shape.getPosition().y + shape.getSize().y) > window.getSize().y)
    {
      speeds[index][1] = -speeds[index][1];
      textSpeedShapes[index + indexTextArr][1] = -textSpeedShapes[index + indexTextArr][1];
    }

    window.draw(shape);
    index++;
  }
}

void drawTexts(std::vector<sf::Text> &shapes, std::vector<std::array<float, 2>> &speeds, sf::RenderWindow &window)
{
  std::size_t index = 0;
  for (auto &shape : shapes)
  {
    shape.move(speeds[index][0], speeds[index][1]);
    window.draw(shape);
    index++;
  }
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
        handleWindowProps(iss, window_width, window_height);
      }
      if (word == "Font")
      {
        handleFontProps(iss, font, font_size, font_color);
      }
      if (word == "Circle")
      {
        handleCircleProps(iss, font, font_size, font_color, circle_shapes, circle_shapes_speeds, text_shapes, text_shapes_speeds);
      }
      if (word == "Rectangle")
      {
        handleRectangleProps(iss, font, font_size, font_color, rectangle_shapes, rectangle_shapes_speeds, text_shapes, text_shapes_speeds);
      }
    }
  }
  file.close();

  auto window =
      sf::RenderWindow{{window_width, window_height}, "First Assignment"};

  window.setFramerateLimit(244);

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
    drawCircles(circle_shapes, circle_shapes_speeds, text_shapes_speeds, window, index);
    index = index + circle_shapes.size();
    drawRectangles(rectangle_shapes, rectangle_shapes_speeds, text_shapes_speeds, window, index);
    drawTexts(text_shapes, text_shapes_speeds, window);

    window.display();
  }
}
