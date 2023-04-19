#include "RegularMesh.hpp"
#include <vector>
#include <string>
#include <cmath>

RegularMesh::RegularMesh(int rows_, int cols_, float spacing_)
{
  spacing = spacing_;
  rows = rows_;
  cols = cols_;
  cells.reserve(rows * cols);
  cellsOnPlan.reserve(rows * cols);
  /* This isn't perfect, but I just want to get something down for now*/
  vertices.reserve(2 * rows * cols);
  edges.reserve(2 * rows * cols);
  /* TODO: Reserve the connectivity arrays for performance */
}

RegularMesh::triangulate()
{
}

RegularMesh::generateCells()
{
  float offset = 0;
  for (i = 0; i < cols; i++){
    offset = (i % 2) * (spacing / 2.);
    for (j = 0; j < rows; j++){
      Point2D point = Point2D();
      point.x = spacing * i + 1.;
      point.y = spacing * (sqrt(3) / 2.) * row + 1. + offset;
      cellsOnPlane.emplace_back(point);
    }
  }
}

RegularMesh::generateVoronoi()
{
}

RegularMesh::writeNetCDF(std::string filename)
{
}
