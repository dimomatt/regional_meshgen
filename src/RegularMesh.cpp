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
  //
  /* 
   * This loops over every triangle and pushes the cells on cell to shared 
   * vertices, the cells on verte to cells on vertex,
   * and the vertices on cell to vertices on cell. 
   *
   */
  this->verticesOnPlane.resize(out.numberoftriangles);
  for (int i=0; i < out.numberoftriangles; i++){
    int a = out.trianglelist[i * 3];
    int b = out.trianglelist[i * 3 + 1];
    int c = out.trianglelist[i * 3 + 2];
    Point2D pointA = {out.pointlist[2*a], out.pointlist[2*a + 1]};
    Point2D pointB = {out.pointlist[2*b], out.pointlist[2*b + 1]};
    Point2D pointC = {out.pointlist[2*c], out.pointlist[2*c + 1]};
    
    // Get the vertices on the plane to later project
    this->verticesOnPlane[i] = (circumcenter2D( pointA, pointB, pointC));

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
  this->cellsOnCell = convertSetsToVectors(sharedVertices);
  this->verticesOnCell = convertSetsToVectors(verticesOnCell);
  this->cellsOnVertex = convertSetsToVectors(cellsOnVertex);

  // Populate nEdgesOnCell
  this->nEdgesOnCell.resize(this->cellsOnCell.size());
  for (int i = 0; i < this->cellsOnCell.size(); i++){
    this->nEdgesOnCell[i] = this->cellsOnCell[i].size();
  }


  // CCW ordering in VerticesOnCell
  for (int i=0; i < this->verticesOnCell.size(); i++){
    Point2D cellPoint = this->cellsOnPlane[i];
    std::sort(this->verticesOnCell[i].begin(),
              this->verticesOnCell[i].end(),
              [&](int i1, int i2) {
              // Get the angle from the central cell point to the neighbor
              double angle1 = getAngle(cellPoint, this->verticesOnPlane[i1]);
              double angle2 = getAngle(cellPoint, this->verticesOnPlane[i2]);

              return angle1 < angle2;
              });
  }
  // CCW ordering in cellsOnCell
  for (int i=0; i < this->cellsOnCell.size(); i++){
    Point2D cellPoint = this->cellsOnPlane[i];
    std::sort(this->cellsOnCell[i].begin(),
              this->cellsOnCell[i].end(),
              [&](int i1, int i2) {
              // Lambda that sorts based on the angle from a reference point
              Point2D p1{out.pointlist[2*i1], out.pointlist[2*i1 + 1]};
              Point2D p2{out.pointlist[2*i2], out.pointlist[2*i2 + 1]};
              
              // Get the angle from the central cell point to the neighbor
              double angle1 = getAngle(cellPoint, p1);
              double angle2 = getAngle(cellPoint, p2);

              return angle1 < angle2;
              });
  }
}

void RegularMesh::projectCells(AbstractProjector& projector){
  this->cells.resize(this->cellsOnPlane.size());
  this->latLonCells.resize(this->cellsOnPlane.size());
  std::transform(this->cellsOnPlane.begin(),
                 this->cellsOnPlane.end(),
                 this->cells.begin(),
                 [&projector](Point2D x){ 
                 return projector.projectToSphere(x); });
  std::transform(this->cells.begin(),
                 this->cells.end(),
                 this->latLonCells.begin(),
                 [&](CartesianPoint x){
                 return this->convertCartesianToLatLon(x);} );
  this->vertices.resize(this->verticesOnPlane.size());
  this->latLonVertices.resize(this->verticesOnPlane.size());
  std::transform(this->verticesOnPlane.begin(),
                 this->verticesOnPlane.end(),
                 this->vertices.begin(),
                 [&projector](Point2D x){ 
                 return projector.projectToSphere(x); });
  std::transform(this->vertices.begin(),
                 this->vertices.end(),
                 this->latLonVertices.begin(),
                 [&](CartesianPoint x){
                 return this->convertCartesianToLatLon(x); });
  this->edges.resize(this->edgesOnPlane.size());
  this->latLonEdges.resize(this->edgesOnPlane.size());
  std::transform(this->edgesOnPlane.begin(),
                 this->edgesOnPlane.end(),
                 this->edges.begin(),
                 [&projector](Point2D x){ 
                 return projector.projectToSphere(x); });
  std::transform(this->edges.begin(),
                 this->edges.end(),
                 this->latLonEdges.begin(),
                 [&](CartesianPoint x){
                 return this->convertCartesianToLatLon(x); });
}

void RegularMesh::generateCells()
{
  float offset = 0;
  int cellIndex = 0;
  this->cellIsOnEdge.resize(rows * cols);
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
  for (int i=1; i < this->indexToCellID.size(); i++){
    this->indexToCellID[i] = i;
  }
}

// This is a helper and will go with helpers in a bit
std::pair<int, int> make_ordered_pair(int a, int b) {
  return std::make_pair(std::min(a, b), std::max(a, b));
}

    
void RegularMesh::generateVoronoi(){
  // cellsOnCell is now in ccw order,
  // we can calculate the edges
  // in ccw order from that.
  this->cellsOnEdge.resize(this->nEdges * 2);
  this->edgesOnCell.resize(this->nCells);
  this->edgesOnVertex.resize(this->nVertices);
  this->verticesOnEdge.resize(this->nEdges * 2);
  int cell = 0;
  std::map<std::pair<int, int>, int> insertedEdges;
  for (auto col : this->verticesOnCell){
    // Get the edge between the first and last vertex on a cell
    std::pair<int,int> orderedPair = make_ordered_pair(col.back(),
        col.front());
    Point2D candidateEdge = midpoint(this->verticesOnPlane[col.back()],
                                     this->verticesOnPlane[col.front()]);
    
    
    // Check if we have seen this before
    auto iterator = insertedEdges.find(orderedPair);
    if (iterator != insertedEdges.end()){
      // If we have seen the edge, then just check the index
      int index = insertedEdges.at(orderedPair);
      // Add the edge index to the edge on cell at current cell
      this->edgesOnCell[cell].push_back(index);
    } else{
      // insert the edge in to the map as a key, with the value being
      // the index of that edge
      insertedEdges.insert({orderedPair, this->edgesOnPlane.size()});
      
      // Insert the edge index in to the edgesOnCell array at the current cell
      this->edgesOnCell[cell].push_back(this->edgesOnPlane.size());

      // Add the edges to the vertices used to construct the edge
      this->edgesOnVertex[col.back()].push_back(this->edgesOnPlane.size());
      this->edgesOnVertex[col.front()].push_back(this->edgesOnPlane.size());
      
      // Update the vertices to ones used to construct the edge
      this->verticesOnEdge[this->edgesOnPlane.size()].push_back(col.front());
      this->verticesOnEdge[this->edgesOnPlane.size()].push_back(col.back());
      
      // Finally, add the edge to the mesh
      this->edgesOnPlane.push_back(candidateEdge);
    }
    for (int i = 0; i < col.size() - 1; i++){
      // Repeat the previous algorithm but for the rest of the elements
      orderedPair = make_ordered_pair(col[i], col[i+1]);    
      candidateEdge = midpoint(this->verticesOnPlane[col[i]],
                              this->verticesOnPlane[col[i+1]]);
      iterator = insertedEdges.find(orderedPair);

      if (iterator != insertedEdges.end()){
        // Insert edge index in to the array at the current cell
        int index = insertedEdges.at(orderedPair);
        this->edgesOnCell[cell].push_back(index);

      } else {
        // If we haven't seen the edge yet,
        // Insert the edge in to the map as a key for the index of the edge
        insertedEdges.insert({orderedPair, this->edgesOnPlane.size()});
        
        // add the edge index to the array for the current cell
        this->edgesOnCell[cell].push_back(this->edgesOnPlane.size());

        // Add the edge to the vertices used to construct the edge
        this->edgesOnVertex[col[i]].push_back(this->edgesOnPlane.size());
        this->edgesOnVertex[col[i + 1]].push_back(this->edgesOnPlane.size());
        
        // Add the vertices used to construct the edge to the current edge
        // array
        this->verticesOnEdge[this->edgesOnPlane.size()].push_back(col[i]);
        this->verticesOnEdge[this->edgesOnPlane.size()].push_back(col[i + 1]);
        
        // finally, add the edge to the mesh
        this->edgesOnPlane.push_back(candidateEdge);
      }
    }
    cell += 1;
  }
  this->nEdges = this->edgesOnPlane.size();
  this->edgesOnVertex.resize(this->nVertices);
  // Get the cells that claim each edge
  for (int cell = 0; cell < this->edgesOnCell.size(); cell++){
    for (auto edge : this->edgesOnCell[cell]){
      this->cellsOnEdge[edge].push_back(cell);
    }
  }
  
  return;
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

void RegularMesh::getEdgeWeights(){
  return;
}

