#ifndef MESH_H
#define MESH_H

#include <vector>

class Mesh
{
  private:
    int maxEdges;
    int maxEdges2;
    int vertexDegree;
    int nCells;
    int nEdges;
    int nVertices;

  public:
    /* Mesh Elements */  
    std::vector<std::vector<float>> cells;
    std::vector<std::vector<float>> vertices;
    std::vector<std::vector<float>> edges;

    /*! Connectivity Fields */
    std::vector<int> nEdgesOnCell;
    std::vector<std::vector<int>> cellsOnCell;
    std::vector<std::vector<int>> edgesOnCell;
    std::vector<std::vector<int>> verticesOnCell;
    std::vector<std::vector<int>> edgesOnVertex;
    std::vector<std::vector<int>> verticesOnEdge;
    std::vector<std::vector<int>> cellsOnvertex;
    std::vector<std::vector<int>> cellsOnEdge;
    
    /*! Write the file to a netcdf */
    virtual void writeNetCDF() = 0;
    
    /*! Generate the initial grid */
    virtual void generateCells() = 0;

    /*! Get the delaunay triangulation of the generating points */
    virtual void triangulate() = 0;

    /*! Get the voronoi diagram of the mesh */
    virtual void generateVoronoi() = 0;

}
