project(regional_meshgen)
cmake_minimum_required(VERSION 3.0)

include_directories(${PROJECT_SOURCE_DIR}/src)
include_directories(${PROJECT_SOURCE_DIR}/src/lib)
add_executable(regional_meshgen ${PROJECT_SOURCE_DIR}/src/main.cpp)


