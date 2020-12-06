cmake_minimum_required(VERSION 3.19.1)

project("k-means")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)

find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)

add_executable(k_means src/main.cpp src/ftools.hpp)

target_link_libraries(
  k_means
  sfml-graphics
  sfml-window
  sfml-system
)
