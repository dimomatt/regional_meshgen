cmake_minimum_required(VERSION 3.5)

project(regional_meshgen VERSION 0.1
                         DESCRIPTION "Regional Mesh generation for MPAS"
                         LANGUAGES CXX C)
add_subdirectory(${PROJECT_SOURCE_DIR}/src)
add_subdirectory(${PROJECT_SOURCE_DIR}/app)
add_subdirectory(${PROJECT_SOURCE_DIR}/libs/triangle)



