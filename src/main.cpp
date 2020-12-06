#include <vector>
#include <utility>
#include <SFML/Graphics.hpp>
#include "ftools.hpp"

#define DATA_FILENAME "points.ssv"

int main() {
  sf::VertexArray data_points(sf::Points);

  std::cout << "Loding data points..." << std::endl;
  ftools::readFromFile(DATA_FILENAME, &data_points);
  std::cout << "Loaded data points." << std::endl;

  size_t num_clusters;
  std::cout << "How many clusters should be calculated?" << std::endl;
  std::cin >> num_clusters;

  std::cout << "Calculating k-means with " << num_clusters << " clusters.\n";

  sf::RenderWindow window(sf::VideoMode(1280, 720), "K-means Clustering");
  window.setFramerateLimit(5);

  sf::View view(sf::FloatRect(58.f, 80.f, 260.f, 170.f));

  bool mouse_moving = false;
  sf::Vector2f oldPos;

  while (window.isOpen()) {
    sf::Event event;

    while (window.pollEvent(event)) {
      switch (event.type) {
        case sf::Event::Closed:
          window.close();
          break;

        case sf::Event::MouseWheelMoved:
          view.zoom(1.f + event.mouseWheel.delta * -0.05f);
          break;

        case sf::Event::MouseButtonPressed:
          if (event.mouseButton.button == 0) {
            mouse_moving = true;
            oldPos = window.mapPixelToCoords(sf::Vector2i(event.mouseButton.x,
                                             event.mouseButton.y));
          }
          break;

        case sf::Event::MouseButtonReleased:
          if (event.mouseButton.button == 0)
            mouse_moving = false;
          break;

        case sf::Event::MouseMoved:
          {
            if (!mouse_moving)
              break;

            const sf::Vector2f newPos = window.mapPixelToCoords(sf::Vector2i(event.mouseMove.x, event.mouseMove.y));
            const sf::Vector2f deltaPos = oldPos - newPos;

            view.setCenter(view.getCenter() + deltaPos);
            window.setView(view);

            oldPos = window.mapPixelToCoords(sf::Vector2i(event.mouseMove.x, event.mouseMove.y));
            break;
          }
        default:;
      }
    }

  window.setView(view);
    window.clear();
    window.draw(data_points);
    window.display();
  }

  return 0;
}
