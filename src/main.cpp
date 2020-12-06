#include <float.h>
#include <math.h>
#include <SFML/Graphics.hpp>
#include "ftools.hpp"

#define DATA_FILENAME "points.ssv"
#define MAX_ITERATIONS 100

sf::Color colors[] = {
  sf::Color::Red,
  sf::Color::Cyan,
  sf::Color::Magenta,
  sf::Color::Yellow,
  sf::Color::Blue,
  sf::Color::Green,
  sf::Color::White,
};

sf::VertexArray data_points(sf::Points);
std::vector<size_t> point_cluster_kind;
std::vector<sf::Vertex> means;
size_t vertexCount;
size_t num_clusters;
bool should_calculate;

inline double euclidean_distance(const sf::Vertex&, const sf::Vertex&);
void kmeans_iteration();

int main() {
  srand( time(NULL) );

  std::cout << "Loding data points..." << std::endl;
  ftools::readFromFile(DATA_FILENAME, &data_points);
  std::cout << "Loaded data points." << std::endl;

  vertexCount = data_points.getVertexCount();
  point_cluster_kind.resize(data_points.getVertexCount());

  std::cout << "How many clusters should be calculated?" << std::endl;
  std::cin >> num_clusters;
  std::cout << "Calculating k-means with " << num_clusters << " clusters.\n";

  means.resize(num_clusters);
  for (size_t i = 0; i < num_clusters; ++i) {
    means[i] = data_points[rand() % vertexCount];
    means[i].color = colors[i%7];
    // means[i].color = sf::Color(sf::Color(rand() % 156 + 100, rand() % 156 + 100,
                                         // rand() % 156 + 100));
  }

  sf::RenderWindow window(sf::VideoMode(1280, 720), "K-means Clustering");
  window.setFramerateLimit(30);

  sf::View view(sf::FloatRect(0.f, 0.f, 110.f, 55.f));
  view.setCenter(-44.f, 20.f);

  bool mouse_moving = false;
  sf::Vector2f oldPos;

  should_calculate = true;
  size_t iterations = 0;
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

        case sf::Event::MouseMoved: {
          if (!mouse_moving)
            break;

          const sf::Vector2f newPos = window.mapPixelToCoords(sf::Vector2i(
                                        event.mouseMove.x, event.mouseMove.y));
          const sf::Vector2f deltaPos = oldPos - newPos;
          view.setCenter(view.getCenter() + deltaPos);
          oldPos = window.mapPixelToCoords(sf::Vector2i(event.mouseMove.x,
                                           event.mouseMove.y));
          break;
        }

        case sf::Event::KeyReleased: {
          if (event.key.code == sf::Keyboard::X) {
            view.reset(sf::FloatRect(0.f, 0.f, 0.453083f, 0.226542f));
            view.setCenter(-73.9336f, 40.741f);
          }

          if (event.key.code == sf::Keyboard::Z) {
            view.reset(sf::FloatRect(0.f, 0.f, 110.f, 55.f));
            view.setCenter(-44.f, 20.f);
          }

          break;
        }

        default:
          ;
      }
    }

    window.setView(view);
    window.clear();
    window.draw(data_points);
    window.display();
    sf::sleep(sf::milliseconds(500));

    if (iterations < MAX_ITERATIONS && should_calculate) {
      ++iterations;
      kmeans_iteration();
    }
  }

  return 0;
}

inline double euclidean_distance(const sf::Vertex& a, const sf::Vertex& b) {
  double x = a.position.x - b.position.x;
  double y = a.position.y - b.position.y;
  return sqrt(x*x + y*y);
}

void kmeans_iteration() {
  size_t i, j;
  double min_dist, dist;

  for (i = 0; i < vertexCount; ++i) {
    min_dist = DBL_MAX;

    for (j = 0; j < num_clusters; ++j) {
      dist = euclidean_distance(data_points[i], means[j]);

      if (dist < min_dist) {
        min_dist = dist;
        data_points[i].color = means[j].color;
        point_cluster_kind[i] = j;
      }
    }
  }

  for (j = 0; j < num_clusters; ++j)
    means[j].position = sf::Vector2f(0, 0);

  std::vector<int> elem_count(num_clusters, 0);

  for (i = 0; i < vertexCount; ++i) {
    size_t cluster_kind = point_cluster_kind[i];
    means[cluster_kind].position += data_points[i].position;
    ++elem_count[ cluster_kind ];
  }

  size_t unchanged_clusters = 0;

  for (j = 0; j < num_clusters; ++j) {
    sf::Vector2f old_pos = means[j].position;
    means[j].position.x /= elem_count[j];
    means[j].position.y /= elem_count[j];

    if (means[j].position == old_pos)
      ++unchanged_clusters;
  }

  if (unchanged_clusters == num_clusters)
    should_calculate = false;
}
