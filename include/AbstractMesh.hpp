#ifndef ABSTRACTMESH_H
#define ABSTRACTMESH_H

#include <vector>
#include <string>
#include "point.hpp"

class AbstractMesh
{
  private:
    int maxEdges;
    int maxEdges2;
    int vertexDegree;
    int nCells;
    int nEdges;
    int nVertices;

  public:
    int rows;
    int cols;
    int resolution;
    
    /* Mesh Elements */
    std::vector<Point2D> cellsOnPlane;  
    std::vector<CartesianPoint> cells;
    std::vector<CartesianPoint> vertices;
    std::vector<CartesianPoint> edges;

    /*! Connectivity Fields */
    std::vector<int> nEdgesOnCell;
    std::vector< std::vector<int> > cellsOnCell;
    std::vector< std::vector<int> > edgesOnCell;
    std::vector< std::vector<int> > verticesOnCell;
    std::vector< std::vector<int> > edgesOnVertex;
    std::vector< std::vector<int> > verticesOnEdge;
    std::vector< std::vector<int> > cellsOnvertex;
    std::vector< std::vector<int> > cellsOnEdge;
    
    /*! Write the file to a netcdf */
    virtual void writeNetCDF(const std::string& filename) = 0;
    
    /*! Generate the initial grid */
    virtual void generateCells() = 0;

    /*! Get the delaunay triangulation of the generating points */
    virtual void triangulate() = 0;

    /*! Get the voronoi diagram of the mesh */
    virtual void generateVoronoi() = 0;

};
#endif // ABSTRACTMESH_H
