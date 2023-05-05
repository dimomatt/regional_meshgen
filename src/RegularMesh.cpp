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
  struct triangulateio in, mid, out, vorout;
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
  vorout.pointlist = NULL;
  vorout.edgelist = NULL;
  vorout.normlist = NULL;
  
  // Index from zero, generate voronoi diagram, and a neighbor list
  char triswitches[] = "zvn";

  // Run the actual triangulate program
  triangulate(triswitches, &in, &out, &vorout);
  std::cout << "Done Triangulating" << std::endl;
  
  // Save the Voronoi edges 
  for (int i = 0; i < vorout.numberofedges; i++) {
    auto one = vorout.edgelist[i * 2];
    auto two = vorout.edgelist[i * 2 + 1];
    // Make points for each end of the edge and save them
    Point2D vertexOne = {vorout.pointlist[one * 2],
                         vorout.pointlist[one * 2 + 1]}; 
    Point2D vertexTwo = {vorout.pointlist[two * 2],
                         vorout.pointlist[two * 2 + 1]};
    /*
    The edge endpoints are the circumcenter of the triangles
     as well as the vertices of the cells
    */
    this->verticesOnPlane.push_back(vertexOne);
    this->verticesOnPlane.push_back(vertexTwo);
    // The midpoint of each edge is stored as the "edge"
    this->edgesOnPlane.push_back(midpoint(vertexOne,
                                          vertexTwo));
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

