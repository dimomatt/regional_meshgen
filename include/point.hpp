#ifndef POINT_H
#define POINT_H
#include <cmath>
struct CartesianPoint
{
  /*! Represents a point in cartesian space */
  double x, y, z;

  CartesianPoint(double x = 0, double y = 0, double z = 0) :
    x(x), y(y), z(z) {}

  CartesianPoint operator-(const CartesianPoint& rhs) const{
    return CartesianPoint(x - rhs.x, y - rhs.y, z - rhs.z);
  }
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
