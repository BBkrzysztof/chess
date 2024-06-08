cmake_minimum_required(VERSION 3.24)
project(chess)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)



add_executable(chess main.cpp)

set(SFML_STATIC_LIBRARIES TRUE)
set(SFML_DIR C:/SFML/lib/cmake/SFML)

find_package(SFML COMPONENTS system window graphics audio network REQUIRED)


target_link_libraries(chess sfml-system sfml-window sfml-graphics sfml-audio)
