#ifndef REGULARMESH_H
#define REGULARMESH_H

#include "AbstractMesh.hpp"
#include "StereographicProjector.hpp"
#include <string>
#include <vector>

class RegularMesh : public AbstractMesh
{
  private:

    // vector of point indices that are along the edge,
    // these will be thrown out later
    std::vector<bool> cellIsOnEdge;

  public:
    RegularMesh(double radius_, 
                int row_,
                int col_,
                int dx_m,
                int dy_m);

    /*! Generate the initial grid */
    void generateCells();
    
    void addEdge();

    void getEdgesOnEdge();

    void getEdgeWeights();
    
    void pruneBadCells();

    void removeEdge(int edgeIndex);

    void removeCell(int cellIndex);
    
    void removeVertex(int vertexIndex);

    void rotateMeshToLatLong(float latitude, float longitude);
};

#endif
