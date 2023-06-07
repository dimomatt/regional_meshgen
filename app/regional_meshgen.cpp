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

struct CommandLineOptions {
  std::string outFile;
  std::string configFile;
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
  RegularMesh mesh = RegularMesh(3, 4, 1.0);
  StereographicProjector projector(6378.14);
  mesh.generateCells();
  mesh.generateDelaunay();
  mesh.generateVoronoi();
  mesh.projectCells(projector);
  mesh.writeNetCDF(std::string("out.nc"));
}
