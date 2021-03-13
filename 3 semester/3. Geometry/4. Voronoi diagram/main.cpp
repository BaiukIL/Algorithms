#include <iostream>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include "3D/hull.hpp"


double GetAveragePolygonsSidesNum(std::vector<Point3D<double>>& points) {
  // Project points onto paraboloid.
  for (auto& point : points) {
    point.z_ = point.x_*point.x_ + point.y_*point.y_;
  }

  Hull hull(points, true, true);

  std::unordered_map<Point3D<double>*, int> points_sides_count_match;
  points_sides_count_match.reserve(points.size());

  std::unordered_set<Point3D<double>*> border_points =
      hull.GetProjectionPoints();

  for (auto& facet : hull.GetFacets()) {
    for (Point3D<double>* point : facet.points_) {
      if (border_points.count(point) == 0) {
        points_sides_count_match[point] += 2;
      }
    }
  }
  double result = 0;
  for (auto& pair : points_sides_count_match) {
    result += (pair.second / 2.0);
  }
  return result == 0 ? 0 : result / points_sides_count_match.size();
}


int main() {
  std::vector<Point3D<double>> points;

  Point3D<double> point;
  // Read points.
  while(std::cin >> point.x_ >> point.y_) { points.push_back(point); }

  printf("%.6f", GetAveragePolygonsSidesNum(points));
  return 0;
}

