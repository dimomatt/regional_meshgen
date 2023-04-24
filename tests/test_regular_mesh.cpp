#include "catch.hpp"
#include "RegularMesh.hpp"

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
  REQUIRE(mesh.voronoiVertices.size() > 0);
  REQUIRE(mesh.voronoiEdges.size() > 0);
}

TEST_CASE("RegularMesh writes NetCDF file", "[RegularMesh]") {
  RegularMesh mesh(3, 3, 1.0);
  mesh.generateCells();
  mesh.triangulate();
  mesh.generateVoronoi();
  std::string filename = "test.nc";
  mesh.writeNetCDF(filename);
  // TODO: Check that the file was written correctly and delete it
}
