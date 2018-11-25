cmake_minimum_required(VERSION 3.10)

project(GameTest)

INCLUDE_DIRECTORIES(include/)
aux_source_directory(src/ SRC_LIST)
add_executable(${PROJECT_NAME} ${SRC_LIST})

set(CMAKE_CXX_IMPLICIT_LINK_DIRECTORIES /usr/local/lib ${CMAKE_CXX_IMPLICIT_LINK_DIRECTORIES})
target_link_libraries(${PROJECT_NAME} GL glfw GLEW dl libnewton.so pthread entityx)
