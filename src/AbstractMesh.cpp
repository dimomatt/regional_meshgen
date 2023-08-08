#include "AbstractMesh.hpp"
#include "point.hpp"
#include <helpers.hpp>
#include <string>
#include <netcdf>
#include <cmath>
#include <algorithm>

/* 
 * Generates the delaunay triangulation of a set of points.
 */
void AbstractMesh::generateDelaunay()
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
 
  std::vector<std::set<int>> sharedVertices;
  std::vector<std::set<int>> verticesOnCell;
  std::vector<std::set<int>> cellsOnVertex;
  sharedVertices.resize(in.numberofpoints);
  verticesOnCell.resize(in.numberofpoints);
  cellsOnVertex.resize(out.numberoftriangles);
  // This loop is slow at nlogn

void AbstractMesh::generateVoronoi(){
  
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

void AbstractMesh::projectCells(AbstractProjector& projector){
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

void AbstractMesh::writeNetCDF(const std::string& filename)
{
  /*
   * Create File
   */
  netCDF::NcFile outFile(filename, netCDF::NcFile::replace);
  
  /*
   * Define Dimensions
   */
  netCDF::NcDim TWO = outFile.addDim("TWO", 2);
  netCDF::NcDim nCells = outFile.addDim("nCells", this->nCells);
  netCDF::NcDim nEdges = outFile.addDim("nEdges", this->edgesOnPlane.size());
  netCDF::NcDim nVertices = outFile.addDim("nVertices", this->nVertices);
  netCDF::NcDim maxEdges = outFile.addDim("maxEdges", this->maxEdges);
  netCDF::NcDim maxEdges2 = outFile.addDim("maxEdges2", this->maxEdges2);
  netCDF::NcDim vertexDegree = outFile.addDim("vertexDegree",
                                              this->vertexDegree);
  /*
   * Defining Variables
   */
  //debug fields
  netCDF::NcVar planeXCell = outFile.addVar("planeXCell", netCDF::ncDouble, nCells);
  netCDF::NcVar planeYCell = outFile.addVar("planeYCell", netCDF::ncDouble, nCells);
  netCDF::NcVar planeXVertex = outFile.addVar("planeXVertex", netCDF::ncDouble, nVertices);
  netCDF::NcVar planeYVertex = outFile.addVar("planeYVertex", netCDF::ncDouble, nVertices);
  netCDF::NcVar planeXEdge = outFile.addVar("planeXEdge", netCDF::ncDouble, nEdges);
  netCDF::NcVar planeYEdge = outFile.addVar("planeYEdge", netCDF::ncDouble, nEdges);

  // numCells
  netCDF::NcVar indexToCellID = outFile.addVar("indexToCellID", netCDF::ncInt, nCells);
  netCDF::NcVar xCell = outFile.addVar("xCell", netCDF::ncDouble, nCells);
  netCDF::NcVar yCell =  outFile.addVar("yCell", netCDF::ncDouble, nCells);
  netCDF::NcVar zCell = outFile.addVar("zCell", netCDF::ncDouble, nCells);
  netCDF::NcVar latCell = outFile.addVar("latCell", netCDF::ncDouble, nCells);
  netCDF::NcVar longCell = outFile.addVar("longCell", netCDF::ncDouble, nCells);
  netCDF::NcVar nEdgesOnCell = outFile.addVar("nEdgesOnCell", netCDF::ncInt, nCells);
  netCDF::NcVar areaCell = outFile.addVar("areaCell", netCDF::ncDouble, nCells);
  netCDF::NcVar meshDensity = outFile.addVar("meshDensity", netCDF::ncDouble, nCells);
  
  // numEdges 
  netCDF::NcVar indexToEdgeID = outFile.addVar("indexToEdgeID", netCDF::ncInt, nEdges); 
  netCDF::NcVar xEdge = outFile.addVar("xEdge", netCDF::ncDouble, nEdges); 
  netCDF::NcVar yEdge = outFile.addVar("yEdge", netCDF::ncDouble, nEdges); 
  netCDF::NcVar zEdge = outFile.addVar("zEdge", netCDF::ncDouble, nEdges); 
  netCDF::NcVar latEdge = outFile.addVar("latEdge", netCDF::ncDouble, nEdges); 
  netCDF::NcVar lonEdge = outFile.addVar("lonEdge", netCDF::ncDouble, nEdges); 
  netCDF::NcVar nEdgesOnEdge = outFile.addVar("nEdgesOnEdge", netCDF::ncInt, nEdges); 
  netCDF::NcVar dvEdge = outFile.addVar("dvEdge", netCDF::ncDouble, nEdges); 
  netCDF::NcVar dcEdge = outFile.addVar("dcEdge", netCDF::ncDouble, nEdges); 
  netCDF::NcVar angleEdge = outFile.addVar("angleEdge", netCDF::ncDouble, nEdges); 
  
  // nVertices
  netCDF::NcVar indexToVertexID = outFile.addVar("indexToVertexID", netCDF::ncInt, nVertices); 
  netCDF::NcVar xVertex = outFile.addVar("xVertex", netCDF::ncDouble, nVertices); 
  netCDF::NcVar yVertex = outFile.addVar("yVertex", netCDF::ncDouble, nVertices); 
  netCDF::NcVar zVertex = outFile.addVar("zVertex", netCDF::ncDouble, nVertices); 
  netCDF::NcVar latVertex = outFile.addVar("latVertex", netCDF::ncDouble, nVertices); 
  netCDF::NcVar lonVertex = outFile.addVar("lonVertex", netCDF::ncDouble, nVertices); 
  netCDF::NcVar areaTriangle = outFile.addVar("areaTriangle", netCDF::ncDouble, nVertices); 
  
  // nCells, maxEdges
  netCDF::NcVar cellsOnCell = outFile.addVar("cellsOnCell", netCDF::ncInt, {nCells, maxEdges});
  netCDF::NcVar edgesOnCell = outFile.addVar("edgesOnCell", netCDF::ncInt, {nCells, maxEdges});
  netCDF::NcVar verticesOnCell = outFile.addVar("verticesOnCell", netCDF::ncInt, {nCells, maxEdges});
  
  // nEdges, TWO
  netCDF::NcVar cellsOnEdge = outFile.addVar("cellsOnEdge", netCDF::ncInt, {nEdges, TWO});
  netCDF::NcVar verticesOnEdge = outFile.addVar("verticesOnEdge", netCDF::ncInt, {nEdges, TWO});
  
  // nEdges, maxEdges2
  netCDF::NcVar edgesOnEdge = outFile.addVar("edgesOnEdge", netCDF::ncInt, {nEdges, maxEdges2});
  netCDF::NcVar weightsOnEdge = outFile.addVar("weightsOnEdge", netCDF::ncDouble,
                                             {nEdges, maxEdges2});
  
  // nVertices, vertexDegree
  netCDF::NcVar edgesOnVertex = outFile.addVar("edgesOnVertex", netCDF::ncInt,
                                             {nVertices, vertexDegree});
  netCDF::NcVar cellsOnVertex = outFile.addVar("cellsOnVertex", netCDF::ncInt,
                                             {nVertices, vertexDegree});
  netCDF::NcVar kiteAreasOnVertex = outFile.addVar("kiteAreasOnVertex", netCDF::ncInt,
                                                 {nVertices, vertexDegree});
  
  padSubvectors(this->edgesOnEdge, this->maxEdges2, -1);
  print2DVec(this->edgesOnEdge); 
  padSubvectors(this->edgesOnCell, this->maxEdges, -1);
  padSubvectors(this->cellsOnCell, this->maxEdges, -1);
  padSubvectors(this->verticesOnCell, this->maxEdges, -1);
  padSubvectors(this->cellsOnVertex, this->vertexDegree, -1); 
  // debug
  std::vector<double> xPlaneCellIn(this->cells.size());
  std::vector<double> yPlaneCellIn(this->cells.size());
  for (size_t i=0; i < this->cellsOnPlane.size(); i++){
    xPlaneCellIn[i] = this->cellsOnPlane[i].x; 
    yPlaneCellIn[i] = this->cellsOnPlane[i].y; 
  }
  planeXCell.putVar(xPlaneCellIn.data());
  planeYCell.putVar(yPlaneCellIn.data());
  
  std::vector<double> xPlaneVertexIn(this->vertices.size());
  std::vector<double> yPlaneVertexIn(this->vertices.size());
  for (size_t i=0; i < this->verticesOnPlane.size(); i++){
    xPlaneVertexIn[i] = this->verticesOnPlane[i].x; 
    yPlaneVertexIn[i] = this->verticesOnPlane[i].y; 
  }
  planeXVertex.putVar(xPlaneVertexIn.data());
  planeYVertex.putVar(yPlaneVertexIn.data());
  
  std::vector<double> xPlaneEdgeIn(this->edges.size());
  std::vector<double> yPlaneEdgeIn(this->edges.size());
  for (size_t i=0; i < this->edgesOnPlane.size(); i++){
    xPlaneEdgeIn[i] = this->edgesOnPlane[i].x; 
    yPlaneEdgeIn[i] = this->edgesOnPlane[i].y; 
  }
  planeXEdge.putVar(xPlaneEdgeIn.data());
  planeYEdge.putVar(yPlaneEdgeIn.data());
  
  // Write variable
  try {
    std::vector<double> latCellIn(this->cells.size());
    std::vector<double> lonCellIn(this->cells.size());
    std::vector<double> xCellIn(this->cells.size());
    std::vector<double> yCellIn(this->cells.size());
    std::vector<double> zCellIn(this->cells.size());
    
    for (size_t i=0; i < this->cells.size(); i++) {
      this->indexToCellID[i] = i;
      CartesianPoint cell = this->cells[i];
      xCellIn[i] = cell.x;
      yCellIn[i] = cell.y;
      zCellIn[i] = cell.z;
      latCellIn[i] = this->latLonCells[i].lat;
      lonCellIn[i] = this->latLonCells[i].lon;
    }
    xCell.putVar(xCellIn.data());
    yCell.putVar(yCellIn.data());
    zCell.putVar(zCellIn.data()); 
    latCell.putVar(latCellIn.data());
    longCell.putVar(lonCellIn.data());
    std::vector<double> xEdgeIn(this->edges.size());
    std::vector<double> yEdgeIn(this->edges.size());
    std::vector<double> zEdgeIn(this->edges.size());
    for (size_t i=0; i < this->edges.size(); i++) {
      this->indexToEdgeID[i] = i;
      CartesianPoint edge = this->edges[i];
      xEdgeIn[i] = edge.x;
      yEdgeIn[i] = edge.y;
      zEdgeIn[i] = edge.z;
    }
    xEdge.putVar(xEdgeIn.data());
    yEdge.putVar(yEdgeIn.data());
    zEdge.putVar(zEdgeIn.data());
    std::vector<double> latVertexIn(this->vertices.size());
    std::vector<double> lonVertexIn(this->vertices.size());
    std::vector<double> xVertexIn(this->vertices.size());
    std::vector<double> yVertexIn(this->vertices.size());
    std::vector<double> zVertexIn(this->vertices.size());
    for (size_t i=0; i < this->vertices.size(); i++) {
      this->indexToVertexID[i] = i;
      CartesianPoint vertex = this->vertices[i];
      xVertexIn[i] = vertex.x;
      yVertexIn[i] = vertex.y;
      zVertexIn[i] = vertex.z;
      latVertexIn[i] = this->latLonVertices[i].lat;
      lonVertexIn[i] = this->latLonVertices[i].lon;
    } 
    xVertex.putVar(xVertexIn.data());
    yVertex.putVar(yVertexIn.data());
    zVertex.putVar(zVertexIn.data());
    latVertex.putVar(latVertexIn.data());
    lonVertex.putVar(lonVertexIn.data());

    std::vector<double> latEdgeIn(this->edges.size());
    std::vector<double> lonEdgeIn(this->edges.size());
    for (size_t i=0; i < this->edges.size(); i++) {
      latEdgeIn[i] = this->latLonEdges[i].lat;
      lonEdgeIn[i] = this->latLonEdges[i].lon;
    }  
    latEdge.putVar(latEdgeIn.data());
    lonEdge.putVar(lonEdgeIn.data());
  //areaCell.putVar(this->areaCell.data());
  //areaTriangle.putVar(this->areaTriangle.data());
  //meshDensity.putVar(this->meshDensity.data());
  //dvEdge.putVar(this->dvEdge.data());
  //dcEdge.putVar(this->dcEdge.data());
  //angleEdge.putVar(this->angleEdge.data());
  //kiteAreasOnVertex.putVar(this->kiteAreasOnVertex.data());
  //weightsOnEdge.putVar(this->weightsOnEdge.data());
  
  // Write Connectivity Fields;
    nEdgesOnCell.putVar(this->nEdgesOnCell.data());
    //nEdgesOnEdge.putVar(this->nEdgesOnEdge.data());
    indexToCellID.putVar(this->indexToCellID.data());
    //indexToEdgeID.putVar(this->indexToEdgeID.data());
    //indexToVertexID.putVar(this->indexToVertexID.data()); 
  
    edgesOnEdge.putVar(flatten(this->edgesOnEdge).data());
    cellsOnEdge.putVar(flatten(this->cellsOnEdge).data());
    verticesOnEdge.putVar(flatten(this->verticesOnEdge).data());
    cellsOnCell.putVar(flatten(this->cellsOnCell).data());
    edgesOnCell.putVar(flatten(this->edgesOnCell).data());
    verticesOnCell.putVar(flatten(this->verticesOnCell).data());
    cellsOnVertex.putVar(flatten(this->cellsOnVertex).data());
    edgesOnVertex.putVar(flatten(this->edgesOnVertex).data());
  } catch (netCDF::exceptions::NcException& e) {
    std::cerr << "NetCDF exception: " << e.what() << std::endl;
    outFile.close();
    return;
  }
  std::cout << "Done Writing NetCDF" << std::endl; 
  outFile.close();
}
double AbstractMesh::triangleArea(CartesianPoint a,
                                  CartesianPoint b,
                                  CartesianPoint c)
{
  double semiPerimeter, tanqe, sideA, sideB, sideC;

  sideA = greatCircleDistance(a, b);
  sideB = greatCircleDistance(a, c);
  sideC = greatCircleDistance(b, c);
  
  semiPerimeter = 0.5 * (sideA + sideB + sideC);

  tanqe = sqrt(tan(0.5 * semiPerimeter) *
               tan(0.5 * (semiPerimeter - sideA)) *
               tan(0.5 * (semiPerimeter - sideB)) * 
               tan(0.5 * (semiPerimeter - sideC)));
  return 4.0 * atan(tanqe); 

}

double AbstractMesh::getKiteAreasOnVertex(int vertex){
return -1.0;  
}


LatLonPoint AbstractMesh::convertCartesianToLatLon(CartesianPoint point)
{
  LatLonPoint p;
  p.lat = radiansToDegrees(asin(point.z / this->radius));
  p.lon = radiansToDegrees(atan2(point.x, point.y));
  return p; 
}

double AbstractMesh::planeDistance(CartesianPoint a,
                                   CartesianPoint b)
{
  return sqrt(((a.x - b.x) * (a.x - b.x)) +
              ((a.y - b.y) * (a.y - b.y)) +
              ((a.z - b.z) * (a.z - b.z))); 
}


double AbstractMesh::greatCircleDistance(CartesianPoint a,
                                         CartesianPoint b)
{
  double r, c;
  r = 1.0;

  c = planeDistance(a, b);

  return 2.0 * r * asinl(c / 2.0 * r);
}

CartesianPoint AbstractMesh::crossProduct(CartesianPoint a,
                                          CartesianPoint b)
{
  CartesianPoint cross;
  cross.x = a.y * b.z - a.z * b.y;
  cross.y = a.z * b.x - a.x * b.z;
  cross.z = a.x * b.y - a.y * b.x;
  return cross;
}

// Get angle between vector AB and AC on sphere
double AbstractMesh::getSphereAngle(CartesianPoint a,
                                    CartesianPoint b,
                                    CartesianPoint c)
{
  double A, B, C, s, sin_angle;
  CartesianPoint ab, ac;

  CartesianPoint abCrossAc;

  A = greatCircleDistance(b, c);
  B = greatCircleDistance(a, c);
  C = greatCircleDistance(a, b);
  
  ab = b - a;
  ac = c - a;

  abCrossAc = crossProduct(ab, ac);
  s = 0.5 * (A + B + C);
  sin_angle = sqrt(std::min(1.0,
                       std::max(0.0,
                          (sin(s-B) * sin(s-C) / sin(B)*sin(C)))));
  if (dotProduct(abCrossAc, a) >= 0.0) {
    return 2.0 * asin(std::max(std::min(sin_angle, 1.0), -1.0));
  }
  else {
    return -2.0 * asin(std::max(std::min(sin_angle, 1.0), -1.0));
  }
}

double AbstractMesh::dotProduct(CartesianPoint a,
                                CartesianPoint b)
{
  return (a.x * b.x + a.y * b.y + a.z * b.z);
}
