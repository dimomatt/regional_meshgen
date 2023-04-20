cmake_minimum_required(VERSION 3.5)

project(regional_meshgen VERSION 0.1
                         DESCRIPTION "Regional Mesh generation for MPAS"
                         LANGUAGES CXX C
                         )
set(CMAKE_CXX_STANDARD 14)
                       
add_subdirectory(${PROJECT_SOURCE_DIR}/src)
add_subdirectory(${PROJECT_SOURCE_DIR}/app)
add_subdirectory(${PROJECT_SOURCE_DIR}/libs/triangle)

include(FetchContent)
FetchContent_Declare(
    googletest
      URL
      https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
      )

