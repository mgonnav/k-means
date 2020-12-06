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

  sf::RenderWindow window(sf::VideoMode(400, 200), "K-means Clustering");
  window.setFramerateLimit(24);

  while (window.isOpen()) {
    sf::Event event;

    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed)
        window.close();
    }

    window.clear();
    window.draw(data_points);
    window.display();
  }

  return 0;
}
