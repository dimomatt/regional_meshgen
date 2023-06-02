#ifndef FILEMESH_H
#define FILEMESH_H

#include "AbstractMesh.hpp"
#include <string>

class FileMesh : public AbstractMesh
{
  public:
    FileMesh(std::string filename);

    /*! Generate the initial grid */
    void generateCells();

    void generateDelaunay();

    /*! Get the voronoi diagram of the mesh */
    void generateVoronoi();
    
    /*! Project Cells down to a sphere using a projector */
    void projectCells(AbstractProjector& projector);
};

#endif
