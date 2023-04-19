file(GLOB HEADERS CONFIGURE_DEPENDS "${CMAKE_SOURCE_DIR}/include/*.hpp")

add_library(lib OBJECT
            StereographicProjector.cpp
            RegularMesh.cpp
            ${HEADERS})


target_include_directories(lib PUBLIC ../include)
target_include_directories(lib PUBLIC ../libs/triangle)
target_link_libraries(lib triangle)
