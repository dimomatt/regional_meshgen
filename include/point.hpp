#ifndef POINT_H
#define POINT_H
#include <cmath>
struct CartesianPoint
{
  /*! Represents a point in cartesian space */
  double x, y, z;
};

struct Point2D
{
  double x, y;

  bool operator<(const Point &other) const {
    const double epsilon = 1e-7; 
    if (std::abs(x - other.x) < epsilon) {
      return y < other.y;
    }
   return x < other.x; 
  }
};

struct Point2DCompare
{
  
  static constexpr double epsilon = 1e-5;
  bool operator() (const Point2D& lhs, const Point2D& rhs) const
  {
    if (std::abs(lhs.x - rhs.x) > epsilon) {
      return lhs.x < rhs.x;
    } else if (std::abs(lhs.x - rhs.x) > epsilon) {
      return lhs.y < rhs.y;
    } else {
      return false;
    }
  }
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
