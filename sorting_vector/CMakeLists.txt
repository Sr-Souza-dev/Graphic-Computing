cmake_minimum_required(VERSION 3.16)
project(square)
add_executable(${PROJECT_NAME} main.cpp src/camera.cpp src/shaders.cpp src/figure.cpp)
target_link_libraries(${PROJECT_NAME} GL glfw GLEW)

add_custom_command(TARGET square POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E create_symlink "${CMAKE_SOURCE_DIR}/shaders" "${CMAKE_BINARY_DIR}/shaders"
)