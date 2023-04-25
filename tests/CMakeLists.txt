add_executable(tests tests.cpp)
add_library(test_sources OBJECT test_regular_mesh.cpp)

target_link_libraries(tests PRIVATE Catch2::Catch2WithMain lib)
list(APPEND CMAKE_MODULE_PATH ${catch2_SOURCE_DIR}/extras)
include(CTest)
include(Catch)
catch_discover_tests(tests)

