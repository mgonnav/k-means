#include <float.h>
#include <math.h>
#include <utility>
#include <vector>
#include <SFML/Graphics.hpp>
#include "ftools.hpp"

#define DATA_FILENAME "points.ssv"

sf::Color colors[] = {
  sf::Color::Red,
  sf::Color::Cyan,
  sf::Color::Magenta,
  sf::Color::Yellow,
  sf::Color::Blue,
  sf::Color::Green,
  sf::Color::White,
};

double euclidean_distance(const sf::Vertex&, const sf::Vertex&);
void kmeans(sf::VertexArray&, int);

int main() {
  sf::VertexArray data_points(sf::Points);
  std::cout << "Loding data points..." << std::endl;
  ftools::readFromFile(DATA_FILENAME, &data_points);
  std::cout << "Loaded data points." << std::endl;
  size_t num_clusters;
  std::cout << "How many clusters should be calculated?" << std::endl;
  std::cin >> num_clusters;
  std::cout << "Calculating k-means with " << num_clusters << " clusters.\n";
  // kmeans(data_points, num_clusters);
  sf::RenderWindow window(sf::VideoMode(1280, 720), "K-means Clustering");
  window.setFramerateLimit(5);
  sf::View view(sf::FloatRect(58.f, 80.f, 260.f, 170.f));
  bool mouse_moving = false;
  sf::Vector2f oldPos;
  // std::vector<int> initial_centroids(num_clusters);
  std::vector<sf::Vertex*> initial_centroids(num_clusters);

  for (size_t i = 0; i < num_clusters; ++i) {
    initial_centroids[i] = &data_points[rand() % data_points.getVertexCount()];
    initial_centroids[i]->color = sf::Color(colors[i]);
    // initial_centroids[i]->color = sf::Color(rand()%156 + 100, rand()%156 + 100,
                                            // rand()%156 + 100);
    // std::cout << "Color for centroid " << i << ": " << (int)initial_centroids[i]->color.r << " " << (int)initial_centroids[i]->color.g << " " << (int)initial_centroids[i]->color.b << "\n";;
  }

  size_t vertexCount = data_points.getVertexCount();
  double dist, min_dist;
  size_t i, j;

  for (i = 0; i < vertexCount; ++i) {
    min_dist = DBL_MAX;

    for (j = 0; j < num_clusters; ++j) {
      dist = euclidean_distance(data_points[i], *initial_centroids[j]);

      if (dist < min_dist) {
        min_dist = dist;
        data_points[i].color = initial_centroids[j]->color;
        // std::cout << "Color for data point " << i << ": " << (int)data_points[i].color.r << " " << (int)data_points[i].color.g << " " << (int)data_points[i].color.b << "\n";;
      }
    }
  }

  std::vector<sf::Vertex> means(num_clusters);
  for (j = 0; j < num_clusters; ++j)
    means[j].color = initial_centroids[j]->color;

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
          window.setView(view);
          oldPos = window.mapPixelToCoords(sf::Vector2i(event.mouseMove.x,
                                           event.mouseMove.y));
          break;
        }

        default:
          ;
      }
    }


    for (i = 0; i < num_clusters; ++i) {
      means[i].position.x = 0;
      means[i].position.y = 0;
    }

    std::vector<int> elem_count(num_clusters, 0);

    for (i = 0; i < vertexCount; ++i) {
      for (j = 0; j < num_clusters; ++j) {
        if (means[j].color == data_points[i].color) {
          means[j].position.x += data_points[i].position.x;
          means[j].position.y += data_points[i].position.y;
          ++elem_count[j];
          continue;
        }
      }
    }

    for (j = 0; j < num_clusters; ++j) {
      means[j].position.x = means[j].position.x / elem_count[j];
      means[j].position.y = means[j].position.y / elem_count[j];
    }

    for (i = 0; i < vertexCount; ++i) {
      for (j = 0; j < num_clusters; ++j)
        if (data_points[j].color == means[j].color)
          min_dist = euclidean_distance(data_points[j], means[j]);

      for (j = 0; j < num_clusters; ++j) {
        dist = euclidean_distance(data_points[i], means[j]);

        if (dist < min_dist) {
          min_dist = dist;

          if (data_points[i].color != means[j].color) {
            data_points[i].color = means[j].color;
          }

          // std::cout << "Color for data point " << i << ": " << (int)data_points[i].color.r << " " << (int)data_points[i].color.g << " " << (int)data_points[i].color.b << "\n";;
        }
      }
    }

    window.setView(view);
    window.clear();
    window.draw(data_points);
    window.display();
  }

  return 0;
}

inline double euclidean_distance(const sf::Vertex& a, const sf::Vertex& b) {
  double x = a.position.x - b.position.x;
  double y = a.position.y - b.position.y;
  return sqrt(x*x + y*y);
}

void kmeans(sf::VertexArray& data_points, int num_clusters) {
  // std::vector<int> initial_centroids(num_clusters);
  std::vector<sf::Vertex*> initial_centroids(num_clusters);

  for (int i = 0; i < num_clusters; ++i) {
    initial_centroids[i] = &data_points[rand() % data_points.getVertexCount()];
    initial_centroids[i]->color = sf::Color(rand()%156 + 100, rand()%156 + 100,
                                            rand()%156 + 100);
    // std::cout << "Color for centroid " << i << ": " << (int)initial_centroids[i]->color.r << " " << (int)initial_centroids[i]->color.g << " " << (int)initial_centroids[i]->color.b << "\n";;
  }

  int vertexCount = data_points.getVertexCount();
  double dist, min_dist;
  int i, j;

  for (i = 0; i < vertexCount; ++i) {
    min_dist = DBL_MAX;

    for (j = 0; j < num_clusters; ++j) {
      dist = euclidean_distance(data_points[i], *initial_centroids[j]);

      if (dist < min_dist) {
        min_dist = dist;
        data_points[i].color = initial_centroids[j]->color;
        // std::cout << "Color for data point " << i << ": " << (int)data_points[i].color.r << " " << (int)data_points[i].color.g << " " << (int)data_points[i].color.b << "\n";;
      }
    }
  }

  bool changed = true;
  std::vector<sf::Vertex> means(num_clusters);

  for (j = 0; j < num_clusters; ++j)
    means[j].color = initial_centroids[j]->color;

  while (changed) {
    changed = false;

    for (i = 0; i < num_clusters; ++i) {
      means[i].position.x = 0;
      means[i].position.y = 0;
    }

    std::vector<int> elem_count(num_clusters, 0);

    for (i = 0; i < vertexCount; ++i) {
      for (j = 0; j < num_clusters; ++j) {
        if (means[j].color == data_points[i].color) {
          means[j].position.x += data_points[i].position.x;
          means[j].position.y += data_points[i].position.y;
          ++elem_count[j];
          continue;
        }
      }
    }

    for (j = 0; j < num_clusters; ++j) {
      means[j].position.x = means[j].position.x / elem_count[j];
      means[j].position.y = means[j].position.y / elem_count[j];
    }

    for (i = 0; i < vertexCount; ++i) {
      min_dist = DBL_MAX;

      for (j = 0; j < num_clusters; ++j) {
        dist = euclidean_distance(data_points[i], means[j]);

        if (dist < min_dist) {
          min_dist = dist;

          if (data_points[i].color != means[j].color) {
            data_points[i].color = means[j].color;
            changed = true;
          }

          // std::cout << "Color for data point " << i << ": " << (int)data_points[i].color.r << " " << (int)data_points[i].color.g << " " << (int)data_points[i].color.b << "\n";;
        }
      }
    }
  }
}
