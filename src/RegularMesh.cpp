#include "RegularMesh.hpp"
#include <helpers.hpp>
#include <vector>
#include <string>
#include <cmath>
#include <set>
#include <algorithm>
#include <map>
#include <netcdf>
extern "C" {
#include "triangle.h"
}


RegularMesh::RegularMesh(double radius_,
                         int rows_,
                         int cols_,
                         int dx_m,
                         int dy_m)
{
  // Set some variables
  this->radius = radius_;
  this->dx_m = dx_m;
  this->dy_m = dy_m;
  this->rows = rows_;
  this->cols = cols_;
  this->nCells = (this->rows * this->cols);

  // We know this because it's a hexagonal grid
  this->vertexDegree = 3;
  this->maxEdges = 6;
  this->maxEdges2 = 12;
 
  this->cells.resize(this->rows * this->cols);
  this->cellsOnPlane.resize(this->rows * this->cols);
}


void RegularMesh::generateCells()
{
  float offset = 0;
  int cellIndex = 0;
  this->cellIsOnEdge.resize(this->rows * this->cols);
  for (int i = 0; i < this->cols; i++){
    offset = (i % 2) * (this->dy_m / 2.);
    for (int j = 0; j < this->rows; j++){
      Point2D point = Point2D();
      point.x = this->dx_m * i + 1.;
      point.y = this->dy_m * j + 1. + offset;
      this->cellsOnPlane.push_back(point);

      // Store the edge points to be thrown out after triangulation
      if (j == 0 || i == 0 ||
          j == this->rows - 1 ||
          i == this->cols - 1) {
        this->cellIsOnEdge[cellIndex] = 1;
      }
      cellIndex += 1;
    }
  }
  // Might as well do this here
  for (int i=1; i < this->nCells; i++){
    this->indexToCellID.push_back(i);
  }
}


  

void RegularMesh::rotateMeshToLatLong(float latitude, float longitude){
   std::cout << "Rotating mesh to " << latitude << ", " << longitude << std::endl;
}

void RegularMesh::getEdgesOnEdge(){
std::cout << "Finding edges on edges" << std::endl;
this->edgesOnEdge.resize(this->nEdges);
// Loop over CellsOnEdge
for (int edge = 0; edge < this->cellsOnEdge.size(); edge++){ 
  for (auto cell : this->cellsOnEdge[edge]) {
    // For each of the two cells, loop over EdgesOnCell[cell]
    for (int j = 0; j < this->edgesOnCell[cell].size(); j++){
      int edgeOnCell = this->edgesOnCell[cell][j];
      if (edgeOnCell != edge) {
        this->edgesOnEdge[edge].push_back(edgeOnCell);
      }
    }
  } 
}
}

/* Removes <index> from all subcevtors, and replaces them with the 
 * correct index. For each value greater thaqn index, subtract one.
 */
template <typename T>
void reindexSubvector(int index, std::vector<T>& vector){
  for (auto vec : vector){
    for (int i = 0; i < vec.size(); i++){
      if (vec[i] > index){
        vec[i] -= 1;
      }
      else if (vec[i] == index){
        vec.erase(vec.begin() + i);
      }
    }
  }
  return;
}

void RegularMesh::removeCell(int cellIndex)
{
// Erase a lot of cell elements directly
  this->cellIsOnEdge.erase(this->cellIsOnEdge.begin() + cellIndex);
  this->cellsOnPlane.erase(this->cellsOnPlane.begin() + cellIndex);
  this->cells.erase(this->cells.begin() + cellIndex);
  this->latLonCells.erase(this->latLonCells.begin() + cellIndex);
  //this->areaCell.erase(this->areaCell.begin() + cellIndex);
  //this->nEdgesOnCell.erase(this->nEdgesOnCell.begin() + cellIndex);
  //this->indexToCellID.erase(this->indexToCellID.begin() + cellIndex);
  this->cellsOnCell.erase(this->cellsOnCell.begin() + cellIndex);
  this->edgesOnCell.erase(this->edgesOnCell.begin() + cellIndex);
  this->verticesOnCell.erase(this->verticesOnCell.begin() + cellIndex);
   
  // reindex cellsOnCell
  reindexSubvector(cellIndex, this->cellsOnCell);
  // reindex cellsOnEdge
  reindexSubvector(cellIndex, this->cellsOnEdge);
  // reindex cellsOnVertex
  reindexSubvector(cellIndex, this->cellsOnVertex);
  this->nCells -= 1;
  return;
}


void RegularMesh::pruneBadCells()
{
  int cells = cellIsOnEdge.size();
  int offset = 0;
  for (int cell = 0; cell < this->nCells; cell++){
    if (cellIsOnEdge[cell]){
      removeCell(cell);
      cell -= 1;
    }
  }
  return;
}
/*
void RegularMesh::removeVertex(){

}
*/
//void RegularMesh::removeVertex()

void RegularMesh::getEdgeWeights(){
  return;
}

