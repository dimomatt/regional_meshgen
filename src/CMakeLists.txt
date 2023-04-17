file(GLOB HEADERS CONFIGURE_DEPENDS "${CMAKE_SOURCE_DIR}/include/*.hpp")

add_library(lib OBJECT
            StereographicProjector.cpp
            ${HEADERS})

target_include_directories(lib PUBLIC ../include)
