#ifndef POINT_H
#define POINT_H
struct CartesianPoint
{
  /*! Represents a point in cartesian space */
  double x, y, z;
};

struct Point2D
{
  double x, y;
};

struct SphericalPoint
{
  double r, phi, theta;
};

struct LatLonPoint
{
  double lat, lon;
};
#endif //POINT_H
