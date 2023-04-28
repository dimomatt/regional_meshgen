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

struct sphericalPoint
{
  float r, phi, theta;
};

struct latLonPoint
{
  float lat, lon;
};
#endif //POINT_H
