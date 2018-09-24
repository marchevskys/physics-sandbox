cmake_minimum_required(VERSION 3.1.2)

project(GameTest)


include_directories(include/)
aux_source_directory(src/ SRC_LIST)
add_executable(${PROJECT_NAME} ${SRC_LIST})

#find_package(globjects REQUIRED)
#find_package(glbinding REQUIRED)

target_link_libraries(${PROJECT_NAME} GL glfw dl Newton pthread )
#target_link_libraries(${PROJECT_NAME} GL glbinding globjects glfw dl Newton pthread )
