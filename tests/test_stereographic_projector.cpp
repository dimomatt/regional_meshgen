#include <vector>
#include <cmath>

#include "catch.hpp"
#include "StereographicProjector.hpp"
#include "point.hpp"

TEST_CASE("StereographicProjector::projectToSphere returns correct CartesianPoint", "[StereographicProjector]") {
  StereographicProjector projector;
  
  SECTION("point is at the origin") {
    Point2D point {0, 0};
    CartesianPoint expected {-0, 0, -1};
    CartesianPoint result = projector.projectToSphere(point);
    REQUIRE(result.x == expected.x);
    REQUIRE(result.y == expected.y);
    REQUIRE(result.z == expected.z);
  }
  
  SECTION("point is at x = 1, y = 0") {
    Point2D point {1, 0};
    CartesianPoint expected {1, 0, 0};
    CartesianPoint result = projector.projectToSphere(point);
    REQUIRE(result.x == Approx(expected.x).margin(1e-6));
    REQUIRE(result.y == Approx(expected.y).margin(1e-6));
    REQUIRE(result.z == Approx(expected.z).margin(1e-6));
  }
  
  SECTION("point is at x = 0, y = 1") {
    Point2D point {0, 1};
    CartesianPoint expected {0, 1, 0};
    CartesianPoint result = projector.projectToSphere(point);
    REQUIRE(result.x == Approx(expected.x).margin(1e-6));
    REQUIRE(result.y == Approx(expected.y).margin(1e-6));
    REQUIRE(result.z == Approx(expected.z).margin(1e-6));
  }
}

TEST_CASE("StereographicProjector::projectToPlane returns correct Point2D", "[StereographicProjector]") {
  StereographicProjector projector;
  
  SECTION("point is at the origin") {
    CartesianPoint point {0, 0, -1};
    Point2D expected {0, 0};
    Point2D result = projector.projectToPlane(point);
    REQUIRE(result.x == expected.x);
    REQUIRE(result.y == expected.y);
  }
  
  SECTION("point is at x = 1, y = 0, z = 0") {
    CartesianPoint point {1, 0, 0};
    Point2D expected {INFINITY, 0};
    Point2D result = projector.projectToPlane(point);
    REQUIRE(std::isinf(result.x));
    REQUIRE(result.y == Approx(expected.y).margin(1e-6));
  }
  
  SECTION("point is at x = 0, y = 1, z = 0") {
    CartesianPoint point {0, 1, 0};
    Point2D expected {0, INFINITY};
    Point2D result = projector.projectToPlane(point);
    REQUIRE(result.x == Approx(expected.x).margin(1e-6));
    REQUIRE(std::isinf(result.y));
  }
  
  SECTION("point is at x = 0, y = 0, z = 1") {
    CartesianPoint point {0, 0, 1};
    Point2D expected {0, 0};
    Point2D result = projector.projectToPlane(point);
    REQUIRE(result.x == Approx(expected.x).margin(1e-6));
    REQUIRE(result.y == Approx(expected.y).margin(1e-6));
  }
}
