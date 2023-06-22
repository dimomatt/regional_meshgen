#include <iostream>
#include <fstream>
#include <string>
#include "StereographicProjector.hpp"
#include "RegularMesh.hpp"
#include "point.hpp"
#include "inipp.h"

// In case we want to add other projections
enum class ProjectorType { STEREOGRAPHIC };

// Hold on to the mesh type
enum class MeshType { REGULAR };

struct CommandLineOptions {
  std::string outFile = "out.nc";
  std::string configFile = "config.ini";
  bool verbose = false; 
};

void printUsage(const std::string& programName) {
  std::cout << "Usage: " << programName <<
               " [-o <output_file>] " <<
               " [-v] (optional) " <<
               " [-c <config_file>]" << std::endl;
}

CommandLineOptions parseCommandLine(int argc, char* argv[]){
  CommandLineOptions options;
  for (int i = 1; i < argc; i++) {
    std::string  arg = argv[i];  
    if (arg == "-o" && i + 1 < argc) {
        options.outFile = argv[++i];
    } else if (arg == "-c" && i + 1 < argc) {
        options.configFile = argv[++i];
    } else if (arg == "-v") {
        options.verbose = true;
    } else {
      std::cout << "Invalid Argument: " << arg <<std::endl;
      printUsage(argv[0]);
      exit(1);
    } 
  }
  return options;
}

int main(int argc, char *argv[])
{  
  // Parse Args
  CommandLineOptions options = parseCommandLine(argc, argv);
  std::cout << options.outFile << std::endl; 
  // Parse config
  inipp::Ini<char> ini;
  std::ifstream is(options.configFile);
  ini.parse(is);
  int rows = -1;
  int cols = -1;
  double radius = -1.0;
  double latitude = -1.0;
  double longitude = -1.0;
  double dx_m = -1.0;
  double dy_m = -1.0;
  inipp::get_value(ini.sections["MESH"], "lat", latitude);
  inipp::get_value(ini.sections["MESH"], "long", longitude);
  inipp::get_value(ini.sections["MESH"], "dx_m", dx_m);
  inipp::get_value(ini.sections["MESH"], "dy_m", dy_m);
  inipp::get_value(ini.sections["MESH"], "rows", rows);
  inipp::get_value(ini.sections["MESH"], "cols", cols);
  inipp::get_value(ini.sections["GEO"], "radius", radius);
  RegularMesh mesh = RegularMesh(radius, rows, cols, dx_m, dy_m);
  StereographicProjector projector(radius);
  mesh.generateCells();
  mesh.generateDelaunay();
  mesh.generateVoronoi();
  mesh.getEdgesOnEdge();
  mesh.projectCells(projector);
  mesh.rotateMeshToLatLong(latitude, longitude);
  mesh.writeNetCDF(std::string(options.outFile));
}
