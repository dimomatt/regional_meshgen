#include "AbstractMesh.hpp"
#include <string>
#include <netcdf>


void AbstractMesh::writeNetCDF(const std::string& filename)
{
  netCDF::NcFile outFile("out.nc", netCDF::NcFile::replace);
}
