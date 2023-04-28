#ifndef HELPERS_H
#define HELPERS_H
#include "point.hpp"

Point2D circumcenter2D(Point2D &a, Point2D &b, Point2D &c);

Point2D circumcenter2D(LatLonPoint &a, LatLonPoint &b, LatLonPoint &c);

double kiteArea(double p, double q);

double kiteArea(double a, double b, double theta);

#endif //HELPERS_H
