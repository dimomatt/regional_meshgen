#ifndef REGULARMESH_H
#define REGULARMESH_H

#include "AbstractMesh.hpp"
#include "StereographicProjector.hpp"
#include <string>

class RegularMesh : public AbstractMesh
{
  public:
    RegularMesh(int row_, int col_, float spacing_);

    /*! Generate the initial grid */
    void generateCells();

    void generateDelaunay();

    /*! Get the voronoi diagram of the mesh */
    void generateVoronoi();
    
    /*! Project Cells down to a sphere using a projector */
    void projectCells(AbstractProjector& projector);
};

#endif
