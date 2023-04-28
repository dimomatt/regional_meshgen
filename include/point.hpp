#ifndef POINT_H
#define POINT_H
struct CartesianPoint
{
  /*! Represents a point in cartesian space */
  float x, y, z;
};

struct Point2D
{
  float x, y;
};

struct SphericalPoint
{
  float r, phi, theta;
};

struct LatLonPoint
{
  float lat, lon;
};
#endif //POINT_H
