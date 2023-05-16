#include "RegularMesh.hpp"
#include "helpers.hpp"
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


RegularMesh::RegularMesh(int rows_, int cols_, float resolution_)
{
  // Set some variables
  this->resolution = resolution_;
  this->rows = rows_;
  this->cols = cols_;
  this->nCells = (this->rows * this->cols);

  // We know this because it's a hexagonal grid
  this->vertexDegree = 3;
  this->maxEdges = 6;
  this->maxEdges2 = 12;
  
  // Start reserving things (we might actually not want this)
  this->cells.reserve(this->rows * this->cols);
  this->cellsOnPlane.reserve(this->rows * this->cols);
}

void RegularMesh::generateDelaunay()
{
  struct triangulateio in, out, vorout;
  std::vector<REAL> input_vertices;
  
  // Transform the points in to the preferred triangle format
  for (const auto point : this->cellsOnPlane){
    input_vertices.push_back(point.x);
    input_vertices.push_back(point.y);
  }

  // Initialize a bunch of things that triangulate needs
  in.numberofpoints = this->cellsOnPlane.size();
  in.pointlist = &input_vertices[0];
  in.numberofpointattributes = 0;
  in.pointmarkerlist = NULL;
  in.numberofsegments = 0;
  in.segmentlist = NULL;
  in.segmentmarkerlist = NULL;
  out.pointmarkerlist = NULL;
  out.segmentlist = NULL;
  out.segmentmarkerlist = NULL;  
  out.pointlist = NULL;
  out.trianglelist = NULL;
  out.triangleattributelist = NULL;
  out.trianglearealist = NULL;
  out.neighborlist = NULL;
  out.segmentlist = NULL;
  out.segmentmarkerlist = NULL;
  out.edgemarkerlist = NULL;
  out.normlist = NULL;
  
  // Index from zero, generate neighbor list, stay quiet
  char triswitches[] = "znQ";

  // Run the actual triangulate program
  triangulate(triswitches, &in, &out, &vorout);
  std::cout << "Done Triangulating" << std::endl;
  std::cout << "there are " << out.numberoftriangles << " triangles" << std::endl;
  
  this->nVertices = out.numberoftriangles; 
  this->nEdges = out.numberoftriangles;
  // Start populating some fields 
 
  std::vector<std::set<int>> sharedVertices;
  std::vector<std::set<int>> verticesOnCell;
  std::vector<std::set<int>> cellsOnVertex;
  sharedVertices.resize(in.numberofpoints);
  verticesOnCell.resize(in.numberofpoints);
  cellsOnVertex.resize(out.numberoftriangles);
  // This loop is slow at nlogn
  // it's definitely doable in linear time and memory
  // but sets are easy to write for now
  for (int i=0; i < out.numberoftriangles; i++){
    int a = out.trianglelist[i * 3];
    int b = out.trianglelist[i * 3 + 1];
    int c = out.trianglelist[i * 3 + 2];
    sharedVertices[a].insert(b);
    sharedVertices[a].insert(c);
    sharedVertices[b].insert(a);
    sharedVertices[b].insert(c);
    sharedVertices[c].insert(a);
    sharedVertices[c].insert(b);
    
    // get cellsOnVertex
    cellsOnVertex[i].insert(a);
    cellsOnVertex[i].insert(b);
    cellsOnVertex[i].insert(c);

    // Also populate Vertices On Cell
    verticesOnCell[a].insert(i);
    verticesOnCell[b].insert(i);
    verticesOnCell[c].insert(i); 
  }
  // Convert them from vectors of sets to vectors of vectors
  std::cout << "Saving cellsOnCell" << std::endl;
  this->cellsOnCell = convertSetsToVectors(sharedVertices);
  std::cout << "Saving verticesOnCell" << std::endl;
  this->verticesOnCell = convertSetsToVectors(verticesOnCell);
  std::cout << "Saving cellsOnVertex" << std::endl;
  this->cellsOnVertex = convertSetsToVectors(cellsOnVertex);
  this->nEdgesOnCell.resize(this->cellsOnCell.size());

  // Populate nEdgesOnCell
  for (int i = 0; i < this->cellsOnCell.size(); i++){
    this->nEdgesOnCell[i] = this->cellsOnCell[i].size();
  }

  // Get the cells in counter clockwise order
  for (int i=0; i < this->cellsOnCell.size(); i++){
    Point2D cellPoint = this->cellsOnPlane[i];
    std::sort(this->cellsOnCell[i].begin(),
              this->cellsOnCell[i].end(),
              [&](int i1, int i2) {
              // Lambda that sorts based on the angle from a reference point
              Point2D p1{out.pointlist[2*i1], out.pointlist[2*i1 + 1]};
              Point2D p2{out.pointlist[2*i2], out.pointlist[2*i2 + 1]};

              double angle1 = getAngle(cellPoint, p1);
              double angle2 = getAngle(cellPoint, p2);

              return angle1 < angle2;
              });
  }

}


void RegularMesh::projectCells(AbstractProjector& projector){
  this->cells.resize(this->cellsOnPlane.size());
  std::transform(this->cellsOnPlane.begin(),
                 this->cellsOnPlane.end(),
                 this->cells.begin(),
                 [&projector](Point2D x){ 
                 return projector.projectToSphere(x); });
  this->vertices.resize(this->verticesOnPlane.size());
  std::transform(this->verticesOnPlane.begin(),
                 this->verticesOnPlane.end(),
                 this->vertices.begin(),
                 [&projector](Point2D x){ 
                 return projector.projectToSphere(x); });
  this->edges.resize(this->edgesOnPlane.size());
  std::transform(this->edgesOnPlane.begin(),
                 this->edgesOnPlane.end(),
                 this->edges.begin(),
                 [&projector](Point2D x){ 
                 return projector.projectToSphere(x); });
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

void RegularMesh::generateVoronoi(){
  // cellsOnCell is now is ccw order,
  // we can calculate the vertices and edges
  // in ccw order from that.
  this->verticesOnCell.resize(this->nCells);
  this->edgesOnCell.resize(this->nEdges);

  Point2D lastVertex;
  for (int i=0; i < this->cellsOnCell.size(); i++){
    for (int j=0; j < this->cellsOnCell[i].size(); j++) {
      int nextIndex = (i + 1) % this->cellsOnCell[i].size();
      this->verticesOnPlane.push_back(
          circumcenter2D(this->cellsOnPlane[i],
                         this->cellsOnPlane[this->cellsOnCell[i][j]],
                         this->cellsOnPlane[this->cellsOnCell[i][nextIndex]]));
      this->verticesOnCell[i].push_back(vertices.size());
    }
  
  }
  return;
}
