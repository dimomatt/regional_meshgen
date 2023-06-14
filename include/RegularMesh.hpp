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
    std::vector<int> edgePoints;

  public:
    RegularMesh(int row_, int col_, int dx_m, int dy_m);

    /*! Generate the initial grid */
    void generateCells();
   
    /*! Generate the Delaunay triangulation */
    void generateDelaunay();

    /*! Get the voronoi diagram of the mesh */
    void generateVoronoi();
    
    /*! Project Cells down to a sphere using a projector */
    void projectCells(AbstractProjector& projector);

    void rotateMeshToLatLong(float latitude, float longitude);
};

#endif
