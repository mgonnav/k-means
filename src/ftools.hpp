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

  float start_lon, start_lat;
  std::string reg;
  float min_lon = 400.f;
  float min_lat = 400.f;
  float max_lon = 0.f;
  float max_lat = 0.f;
  // int i = 0;

  // while (getline(points, reg) && i++ < 300) {
  size_t i = 1;
  while (getline(points, reg)) {
    std::stringstream point(reg);
    point >> start_lon >> start_lat;
    min_lon = std::min(min_lon, start_lon);
    min_lat = std::min(min_lat, start_lat);
    max_lon = std::max(max_lon, start_lon);
    max_lat = std::max(max_lat, start_lat);
    if (start_lat < -90.f) 
      std::cout << "min lat queals" << start_lat << " in line " << i << "\n";
    if (start_lat > 90.f) 
      std::cout << "max lat queals" << start_lat << " in line " << i << "\n";
    if (start_lon < -180.f) 
      std::cout << "min lon queals" << start_lon << " in line " << i << "\n";
    if (start_lon > 180.f) 
      std::cout << "min lon queals" << start_lon << " in line " << i << "\n";
    ++i;


    data_points->append(sf::Vertex(sf::Vector2f(start_lon + 180, start_lat + 90),
                                      sf::Color::Red));
  }

  std::cout << "Min lat: " << min_lat << "\n";
  std::cout << "Max lat: " << max_lat << "\n";
  std::cout << "Min lon: " << min_lon << "\n";
  std::cout << "Max lon: " << max_lon << "\n";
}

}
