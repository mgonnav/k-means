#pragma once
#include <vector>
#include <utility>
#include <iostream>
#include <fstream>
#include <sstream>
#include <SFML/Graphics.hpp>
#include <algorithm>

namespace ftools {

void readFromFile(std::string filename,
                  sf::VertexArray* data_points) {
  std::ifstream points("../data/" + filename);

  if (!points.is_open()) {
    std::cerr << "ERROR: Couldn't open file data/" << filename << ".\n";
    exit(1);
  }

  double start_lon, start_lat;
  std::string reg;

  int i = 0;
  while (getline(points, reg) && i++ < 100000) {
  // while (getline(points, reg)) {
    std::stringstream point(reg);
    point >> start_lon >> start_lat;

    data_points->append(sf::Vertex(sf::Vector2f(start_lon + 180, start_lat + 90),
                                      sf::Color::White));
  }
}

}
