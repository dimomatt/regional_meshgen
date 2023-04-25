file(GLOB HEADERS CONFIGURE_DEPENDS "${CMAKE_SOURCE_DIR}/include/*.hpp")

message(${HEADERS})
add_library(lib OBJECT
            StereographicProjector.cpp
            AbstractMesh.cpp
            RegularMesh.cpp
            ${HEADERS})


target_include_directories(lib PUBLIC ../include)
target_include_directories(lib PUBLIC ../libs/triangle)
target_link_libraries(lib triangle)
