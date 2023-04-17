#include <vector>
#include "point.h"

class AbstractProjector
{
  public:
    
    // Project a single point from a plane to a globe
    virtual CartesianPoint projectToGlobe(Point2D  &point) = 0;

    // Project a point from a globe to the plane
    virtual Point2D projectToPlane(CartesianPoint &point) = 0;
}
