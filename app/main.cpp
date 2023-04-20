#include <iostream>
#include <fstream>
#include "StereographicProjector.hpp"
#include "RegularMesh.hpp"
#include "point.hpp"

int main(int argc, const char **argv)
{
  std::ofstream file;
  file.open("out.txt");
  RegularMesh mesh = RegularMesh(10, 10, 1.0);
  StereographicProjector projector;
  mesh.generateCells();
  mesh.triangulate();
  mesh.generateVoronoi();
  for (Point2D  i : mesh.cellsOnPlane){
    file << i.x << ", " << i.y << "\n";  
  }
  file.close();
  
  mesh.writeNetCDF(std::string("out.nc"));
}
