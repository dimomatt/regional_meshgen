#ifndef ABSTRACTMESH_H
#define ABSTRACTMESH_H

#include <vector>
#include <string>
#include "point.hpp"

class AbstractMesh
{
  protected:

    /*
     *  MESH DIMENSIONS
     */
    size_t maxEdges;
    size_t maxEdges2;
    size_t vertexDegree;
    size_t nCells;
    size_t nEdges;
    size_t nVertices;

  public:

    /*
     * GENERATING OPTIONS
     */
    size_t rows;
    size_t cols;
    int dy_m;
    int dx_m;
    
    /*
     *  MESH ATTRIBUTES;
     */
    double radius;    
    
    /*
     *  MESH ELEMENTS
     */
    std::vector<Point2D> cellsOnPlane; 
    std::vector<Point2D> edgesOnPlane; 
    std::vector<Point2D> verticesOnPlane; 
    std::vector<CartesianPoint> cells;
    std::vector<CartesianPoint> edges;
    std::vector<CartesianPoint> vertices;
    std::vector<LatLonPoint> latLonCells;
    std::vector<LatLonPoint> latLonEdges;
    std::vector<LatLonPoint> latLonVertices; 
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
    
    void getKiteAreaOnSphere(LatLonPoint a,
                             LatLonPoint b,
                             LatLonPoint c,
                             LatLonPoint d);

    void getTriangleAreaOnSphere(LatLonPoint a,
                                 LatLonPoint b,
                                 LatLonPoint c);

    void getAngleOnSphere(LatLonPoint a,
                          LatLonPoint b,
                          LatLonPoint c);
   
    LatLonPoint convertCartesianToLatLon(CartesianPoint point); 
    /*
     *  VIRTUAL FUNCTIONS
     */
    /*! Generate the initial grid */
    virtual void generateCells() = 0;
    
    /*! Generate Delaunay Triangulation */
    virtual void generateDelaunay() = 0;

    /*! Get the voronoi diagram of the mesh */
    virtual void generateVoronoi() = 0;

};
#endif // ABSTRACTMESH_H
