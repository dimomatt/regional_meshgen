#include <iostream>
#include <fstream>
#include <string>
#include "StereographicProjector.hpp"
#include "RegularMesh.hpp"
#include "point.hpp"

// In case we want to add other projections
enum class ProjectorType { STEREOGRAPHIC };

// Hold on to the mesh type
enum class MeshType { REGULAR };


int main(int argc, const char **argv)
{  
  // Parse Args
  std::string configFilename;
  bool verbose = false;
  
  RegularMesh mesh = RegularMesh(1000, 1000, 1.0);
  StereographicProjector projector;
  mesh.generateCells();
  mesh.generateVoronoi();
  mesh.writeNetCDF(std::string("out.nc"));

}
