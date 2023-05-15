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
  //indexToCellID.putVar(this->indexToCellID.data());
  //indexToEdgeID.putVar(this->indexToEdgeID.data());
  //indexToVertexID.putVar(this->indexToVertexID.data());
  cellsOnCell.putVar(this->cellsOnCell.data());
  //edgesOnCell.putVar(this->edgesOnCell.data());
  verticesOnCell.putVar(this->verticesOnCell.data());
  cellsOnVertex.putVar(this->cellsOnVertex.data());
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
