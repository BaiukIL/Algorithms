#include <cstdio>
#include "2D/point.hpp"
#include "2D/polygon.hpp"
#include "2D/intersection.hpp"


template<typename Scale_t>
int scanf(const char* format, Point2D<Scale_t>& point) {
  scanf(format, &point.x_);
  scanf(format, &point.y_);
  return 0;
}

bool IsLower(const Point2D<double>& point1, const Point2D<double>& point2) {
  return point1.y_ == point2.y_ ? point1.x_ < point2.x_ : point1.y_ < point2.y_;
}

void ReadPolygon(Polygon<double>& polygon) {
  size_t polygon_size = polygon.Size();
  std::vector<Point2D<double>> points(polygon_size);

  size_t min_vertex_pos = 0;
  for (size_t i = 0; i < polygon_size; ++i) {
    scanf("%lf", points[i]);
    if (IsLower(points[i], points[min_vertex_pos])) {
      min_vertex_pos = i;
    }
  }

  size_t pos = min_vertex_pos + 1;
  for (size_t i = 0; i < polygon_size; ++i) {
    polygon.points_[i] = points[--pos];
    if (pos == 0) {
      pos = polygon_size;
    }
  }
}


int main() {
  size_t polygon1_vertex_num = 0;
  size_t polygon2_vertex_num = 0;

  scanf("%lu", &polygon1_vertex_num);
  Polygon<double> polygon1(polygon1_vertex_num);
  ReadPolygon(polygon1);

  scanf("%lu", &polygon2_vertex_num);
  Polygon<double> polygon2(polygon2_vertex_num);
  ReadPolygon(polygon2);

  if (HasIntersection(polygon1, polygon2)) {
    printf("YES");
  } else {
    printf("NO");
  }

  return 0;
}

