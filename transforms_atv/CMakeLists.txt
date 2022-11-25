cmake_minimum_required(VERSION 3.16)
project(atv)
add_executable(${PROJECT_NAME} main.cpp src/figure.cpp)
target_link_libraries(${PROJECT_NAME} GL glfw GLEW)
