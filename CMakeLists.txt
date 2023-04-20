cmake_minimum_required(VERSION 3.5)

project(regional_meshgen VERSION 0.1
                         DESCRIPTION "Regional Mesh generation for MPAS"
                         LANGUAGES CXX C
                         )
set(CMAKE_CXX_STANDARD 14)
option(BUILD_TESTS "Build tests for Regional Mesh Gen" ON)

if(BUILD_TESTS)
  Include(FetchContent)
  FetchContent_Declare(
        Catch2
        GIT_REPOSITORY https://github.com/catchorg/Catch2.git
        GIT_TAG        v3.0.1 # or a later release
        )
  FetchContent_MakeAvailable(Catch2)
  add_subdirectory(${PROJECT_SOURCE_DIR}/tests)
endif()
add_subdirectory(${PROJECT_SOURCE_DIR}/src)
add_subdirectory(${PROJECT_SOURCE_DIR}/app)
add_subdirectory(${PROJECT_SOURCE_DIR}/libs/triangle)
