#include "AbstractMesh.hpp"
#include <string>
#include <netcdf>
#include <algorithm>

void AbstractMesh::writeNetCDF(const std::string& filename)
{
  /*
   * Create File
   */
  netCDF::NcFile outFile("out.nc", netCDF::NcFile::replace);
  
  /*
   * Define Dimensions
   */
  netCDF::NcDim TWO = outFile.addDim("TWO", 2);
  netCDF::NcDim nCells = outFile.addDim("nCells", this->nCells);
  netCDF::NcDim nEdges = outFile.addDim("nEdges", this->nEdges);
  netCDF::NcDim nVertices = outFile.addDim("nVertices", this->nVertices);
  netCDF::NcDim maxEdges = outFile.addDim("maxEdges", this->maxEdges);
  netCDF::NcDim maxEdges2 = outFile.addDim("maxEdges2", this->maxEdges2);
  netCDF::NcDim vertexDegree = outFile.addDim("vertexDegree",
                                              this->vertexDegree);
  /*
   * Defining Variables
   */
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
  netCDF::NcVar VerticesOnEdge = outFile.addVar("verticesOnEdge", netCDF::ncInt, {nEdges, TWO});
  
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
  
  
  // Write variable
  try {
    std::vector<double> xCellIn(this->cells.size());
    std::vector<double> yCellIn(this->cells.size());
    std::vector<double> zCellIn(this->cells.size());
    for (size_t i=0; i < this->cells.size(); i++) {
      CartesianPoint cell = this->cells[i];
      xCellIn[i] = cell.x;
      yCellIn[i] = cell.y;
      zCellIn[i] = cell.z;
    } 
   xCell.putVar(xCellIn.data());
   yCell.putVar(yCellIn.data());
   zCell.putVar(zCellIn.data());
   std::cout << "here" << std::endl;
    /* 
    std::vector<double> xEdgeIn(this->edges.size());
    std::vector<double> yEdgeIn(this->edges.size());
    std::vector<double> zEdgeIn(this->edges.size());
    for (size_t i=0; i < this->edges.size(); i++) {
      CartesianPoint edge = this->edges[i];
      xEdgeIn[i] = edge.x;
      yEdgeIn[i] = edge.y;
      zEdgeIn[i] = edge.z;
    } 
   xEdge.putVar(xEdgeIn.data());
   yEdge.putVar(yEdgeIn.data());
   zEdge.putVar(zEdgeIn.data());
*/

    std::vector<double> xVertexIn(this->vertices.size());
    std::vector<double> yVertexIn(this->vertices.size());
    std::vector<double> zVertexIn(this->vertices.size());
    for (size_t i=0; i < this->vertices.size(); i++) {
      CartesianPoint vertex = this->vertices[i];
      xVertexIn[i] = vertex.x;
      yVertexIn[i] = vertex.y;
      zVertexIn[i] = vertex.z;
    } 
   xVertex.putVar(xVertexIn.data());
   yVertex.putVar(yVertexIn.data());
   zVertex.putVar(zVertexIn.data());
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
  std::cout << "Done Writing NetCDF" << std::endl; 
  //nEdgesOnEdge.putVar(this->nEdgesOnEdge.data());
  //indexToCellID.putVar(this->indexToCellID.data());
  //indexToEdgeID.putVar(this->indexToEdgeID.data());
  //indexToVertexID.putVar(this->indexToVertexID.data());
  
  cellsOnCell.putVar(this->cellsOnCell.data());
  std::cout << "Done Writing NetCDF" << std::endl; 
  //edgesOnCell.putVar(this->edgesOnCell.data());
  verticesOnCell.putVar(this->verticesOnCell.data());
  std::cout << "Done Writing NetCDF" << std::endl; 
  cellsOnVertex.putVar(this->cellsOnVertex.data());
  std::cout << "Done Writing cellsOnVerte" << std::endl; 
  //edgesOnVertex.putVar(this->edgesOnVertex.data());
  } catch (netCDF::exceptions::NcException& e) {
    std::cerr << "NetCDF exception: " << e.what() << std::endl;
    outFile.close();
    return;
  }
  std::cout << "Done Writing NetCDF" << std::endl; 
  outFile.close();
  std::cout << "Done Closing NetCDF" << std::endl;
}
