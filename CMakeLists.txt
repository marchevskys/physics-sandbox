cmake_minimum_required(VERSION 3.0.2)

project(GameTest)


#include_directories(include/)
aux_source_directory(src/ SRC_LIST)
add_executable(${PROJECT_NAME} ${SRC_LIST})
target_link_libraries(${PROJECT_NAME} GL glfw dl Newton pthread openal)
