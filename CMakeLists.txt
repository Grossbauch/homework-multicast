cmake_minimum_required(VERSION 3.10)
project(multicast)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_compile_options(-Wall -Wextra -Wpedantic -fsanitize=address,undefined)
add_link_options(-fsanitize=address,undefined)

add_executable(multicast)
target_sources(multicast PRIVATE
  main.cpp
  config.cpp
  config.hpp
  udp.hpp
  udp.cpp
  serial.cpp
  serial.hpp
)
