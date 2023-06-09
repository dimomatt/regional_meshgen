cmake_minimum_required(VERSION 3.5)

project(regional_meshgen VERSION 0.1
                         DESCRIPTION "Regional Mesh generation for MPAS"
                         LANGUAGES CXX C
                         )
set(CMAKE_CXX_STANDARD 14)
option(BUILD_TESTS "Build tests for Regional Mesh Gen" OFF)


set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS}-L/glade/u/apps/ch/opt/netcdf/4.8.1/gnu/10.1.0/lib -lnetcdf_c++4 -lnetcdf -I/glade/u/apps/ch/opt/netcdf/4.8.1/gnu/10.1.0/include -fPIC")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -I/glade/u/apps/ch/opt/netcdf/4.8.1/gnu/10.1.0/include -fPIC")
if(BUILD_TESTS)
  add_subdirectory(${PROJECT_SOURCE_DIR}/tests)
endif(BUILD_TESTS)
add_subdirectory(${PROJECT_SOURCE_DIR}/src)
add_subdirectory(${PROJECT_SOURCE_DIR}/app)
add_subdirectory(${PROJECT_SOURCE_DIR}/libs/triangle)

add_library(inipp INTERFACE)
target_include_directories(inipp INTERFACE ${PROJECT_SOURCE_DIR}/libs/inipp)
