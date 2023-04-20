#ifndef REGULARMESH_H
#define REGULARMESH_H

#include "AbstractMesh.hpp"
#include <string>

class RegularMesh : public AbstractMesh
{
  public:
    RegularMesh(int row_, int col_, float spacing_);

    /*! Write the file to a netcdf */
    void writeNetCDF(const std::string& filename);
    
    /*! Generate the initial grid */
    void generateCells();

    /*! Get the delaunay triangulation of the generating points */
    void triangulate();

    /*! Get the voronoi diagram of the mesh */
    void generateVoronoi();
};

#endif
