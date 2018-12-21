cmake_minimum_required(VERSION 3.10)

project(GameTest)

INCLUDE_DIRECTORIES(include/)
#INCLUDE_DIRECTORIES(/usr/local/include/dVehicle/)
#INCLUDE_DIRECTORIES(/usr/local/include/dScene/)
#INCLUDE_DIRECTORIES(/usr/local/include/dNewton/)
#INCLUDE_DIRECTORIES(/usr/local/include/dMath/)
#INCLUDE_DIRECTORIES(/usr/local/include/dgPhysics/)
#INCLUDE_DIRECTORIES(/usr/local/include/dgCore/)
#INCLUDE_DIRECTORIES(/usr/local/include/dCustomJoints/)
#INCLUDE_DIRECTORIES(/usr/local/include/dContainers/)
#INCLUDE_DIRECTORIES(/usr/local/include/dAnimation/)
#INCLUDE_DIRECTORIES(/usr/local/include/newton/)

aux_source_directory(src/ SRC_LIST)
add_executable(${PROJECT_NAME} ${SRC_LIST})

set(CMAKE_CXX_IMPLICIT_LINK_DIRECTORIES /usr/local/lib ${CMAKE_CXX_IMPLICIT_LINK_DIRECTORIES})
target_link_libraries(${PROJECT_NAME} GL glfw GLEW dl libnewton.so pthread entityx)
