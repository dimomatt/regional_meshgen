#ifndef ABSTRACTMESH_H
#define ABSTRACTMESH_H

#include <vector>
#include <string>
#include "point.hpp"

class AbstractMesh
{
  private:

    /*
     *  MESH DIMENSIONS
     */
    int maxEdges;
    int maxEdges2;
    int vertexDegree;
    int nCells;
    int nEdges;
    int nVertices;

  public:

    /*
     * GENERATING OPTIONS
     */
    int rows;
    int cols;
    int resolution;
    
    /*
     *  MESH ELEMENTS
     */
    std::vector<Point2D> cellsOnPlane; 
    std::vector<Point2D> edgesOnPlane; 
    std::vector<Point2D> verticesOnPlane; 
    std::vector<CartesianPoint> cells;
    std::vector<CartesianPoint> edges;
    std::vector<CartesianPoint> vertices;
    std::vector<latLonPoint> latLonCells;
    std::vector<latLonPoint> latLonEdges;
    std::vector<latLonPoint> latLonVertices; 
    std::vector<double> areaCell;
    std::vector<double> areaTriangle;
    std::vector<double> meshDensity;
    std::vector<double> dvEdge;
    std::vector<double> dcEdge;
    std::vector<double> angleEdge;
    std::vector< std::vector<double> > kiteAreasOnVertex;
    std::vector< std::vector<double> > weightsOnEdge;
    
    /*
     * CONNECTIVITY FIELDS
     */ 
    std::vector<int> nEdgesOnCell;
    std::vector<int> nEdgesOnEdge;
    std::vector<int> indexToCellID; 
    std::vector<int> indexToEdgeID;
    std::vector<int> indexToVertexID;
    std::vector< std::vector<int> > cellsOnCell;
    std::vector< std::vector<int> > edgesOnCell;
    std::vector< std::vector<int> > verticesOnCell;
    std::vector< std::vector<int> > cellsOnEdge;
    std::vector< std::vector<int> > edgesOnEdge;
    std::vector< std::vector<int> > verticesOnEdge;
    std::vector< std::vector<int> > cellsOnVertex;
    std::vector< std::vector<int> > edgesOnVertex;
   
    /*
     *  CLASS FUNCTIONS  
     */
    /*! Write the file to a netcdf */
    void writeNetCDF(const std::string& filename);
    
    
    /*
     *  VIRTUAL FUNCTIONS
     */
    /*! Generate the initial grid */
    virtual void generateCells() = 0;

    /*! Get the voronoi diagram of the mesh */
    virtual void generateVoronoi() = 0;

};
#endif // ABSTRACTMESH_H
