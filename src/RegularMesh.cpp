#include "RegularMesh.hpp"
#include "helpers.hpp"
#include <vector>
#include <string>
#include <cmath>
#include <set>
#include <netcdf>
extern "C" {
#include "triangle.h"
}


RegularMesh::RegularMesh(int rows_, int cols_, float resolution_)
{
  this->resolution = resolution_;
  this->rows = rows_;
  this->cols = cols_;
  this->cells.reserve(this->rows * this->cols);
  this->cellsOnPlane.reserve(this->rows * this->cols);
  /* This isn't perfect, but I just want to get something down for now*/
  this->vertices.reserve(2 * this->rows * this->cols);
  this->edges.reserve(2 * this->rows * this->cols);
  /* TODO: Reserve the connectivity arrays for performance */
}

void RegularMesh::generateVoronoi()
{
  double cornerRadius = this->resolution/2.0;
  for (auto point : this->cellsOnPlane){
    for (int k = 0; k < 6; k++){
      Point2D vertex;
      double angle = k * M_PI / 3.0;
      vertex.x = point.x + cornerRadius * cos(angle);
      vertex.y = point.y + cornerRadius * sin(angle);
      this->verticesOnPlane.push_back(vertex);
      // Get the edge every other point
      if (k % 2 == 1){
        this->edgesOnPlane.push_back(midpoint(this->verticesOnPlane.back(),
                                              vertex));
      }
    }
  }
}



void RegularMesh::generateCells()
{
  float offset = 0;
  for (int i = 0; i < this->cols; i++){
    offset = (i % 2) * (this->resolution / 2.);
    for (int j = 0; j < this->rows; j++){
      Point2D point = Point2D();
      point.x = this->resolution * i + 1.;
      point.y = this->resolution * (sqrt(3) / 2.) * j + 1. + offset;
      this->cellsOnPlane.push_back(point);
    }
  }
}

