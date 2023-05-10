#include <vector>
#include "point.hpp"

class AbstractProjector
{
  public:
    double radius = 1; 

    // Project a single point from a plane to a globe
    virtual CartesianPoint projectToSphere(Point2D  &point) const = 0;

    // Project a point from a globe to the plane
    virtual Point2D projectToPlane(CartesianPoint &point) const  = 0;
};
