#include "../include/RegularMesh.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("RegularMesh generates cells with correct size", "[RegularMesh]") {
  RegularMesh mesh(3, 3, 1.0);
  mesh.generateCells();
  REQUIRE(mesh.cellsOnPlane.size() == 9);
}

TEST_CASE("RegularMesh triangulates", "[RegularMesh]") {
  RegularMesh mesh(3, 3, 1.0);
  mesh.generateCells();
  mesh.triangulate();
  REQUIRE(mesh.vertices.size() > 0);
  REQUIRE(mesh.edges.size() > 0);
}

TEST_CASE("RegularMesh generates Voronoi diagram", "[RegularMesh]") {
  RegularMesh mesh(3, 3, 1.0);
  mesh.generateCells();
  mesh.triangulate();
  mesh.generateVoronoi();
}

