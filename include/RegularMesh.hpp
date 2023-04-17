#ifndef REGULARMESH_H
#define REGULARMESH_H

#include "AbstractMesh.hpp"
#include <string>

class RegularMesh : public AbstractMesh
{
  public:
    /*! Write the file to a netcdf */
    void writeNetCDF(std::string filename);
    
    /*! Generate the initial grid */
    void generateCells();

    /*! Get the delaunay triangulation of the generating points */
    void triangulate();

    /*! Get the voronoi diagram of the mesh */
    void generateVoronoi();
};

#endif
