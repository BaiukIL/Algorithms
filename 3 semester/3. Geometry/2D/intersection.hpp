#ifndef INTERSECTION_HPP
#define INTERSECTION_HPP

#include "point.hpp"
#include "vector.hpp"
#include "polygon.hpp"


template<typename Scale_t>
bool IsLower(const Point2D<Scale_t>& point1, const Point2D<Scale_t>& point2) {
  return point1.y_ == point2.y_ ? point1.x_ < point2.x_ : point1.y_ < point2.y_;
}

/// Return polygon copy reflected symmetrically from (0, 0).
template<typename Scale_t>
Polygon<Scale_t> GetSymmetricPolygon(Polygon<Scale_t> polygon) {
  size_t polygon_size = polygon.Size();
  Polygon<Scale_t> symmetric_polygon(polygon_size);

  size_t min_vertex_pos = 0;
  for (size_t i = 0; i < polygon_size; ++i) {
    // Reflect every point symmetrically from (0, 0).
    polygon.points_[i] *= -1;
    if (IsLower(polygon.points_[i], polygon.points_[min_vertex_pos])) {
      min_vertex_pos = i;
    }
  }

  size_t pos = min_vertex_pos;
  for (size_t i = 0; i < polygon_size; ++i) {
    symmetric_polygon.points_[i] = polygon.points_[pos++];
    if (pos == polygon_size) {
      pos = 0;
    }
  }

  return symmetric_polygon;
}


template<typename Scale_t>
Polygon<Scale_t> GetMinkowskiSumProduct(const Polygon<Scale_t>& polygon1,
                                        const Polygon<Scale_t>& polygon2) {
  Polygon<Scale_t> sum_polygon(0);

  size_t current_vertex1 = 0;
  size_t current_vertex2 = 0;
  size_t polygon1_size = polygon1.Size();
  size_t polygon2_size = polygon2.Size();

  Vector2D<Scale_t> vec1;
  Vector2D<Scale_t> vec2;
  Point2D<Scale_t> next_point = polygon1.points_[0] + polygon2.points_[0];

  while (current_vertex1 < polygon1_size && current_vertex2 < polygon2_size) {
    sum_polygon.points_.push_back(next_point);

    vec1.Assign(polygon1.points_[current_vertex1],
                polygon1.points_[(current_vertex1 + 1) % polygon1_size]);

    vec2.Assign(polygon2.points_[current_vertex2],
                polygon2.points_[(current_vertex2 + 1) % polygon2_size]);

    double angle1 = GetOXAngle(vec1);
    double angle2 = GetOXAngle(vec2);
    // Note that we add both vec1 and vec2 if angle1 == angle2.
    if (angle1 <= angle2) {
      ++current_vertex1;
      next_point += vec1;
    }
    if (angle2 <= angle1) {
      ++current_vertex2;
      next_point += vec2;
    }
  }

  return sum_polygon;
}

/// Return true if point is in polygon.
template<typename Scale_t>
bool IsInPolygon(const Point2D<Scale_t>& point,
                 const Polygon<Scale_t>& polygon) {

  size_t polygon_size = polygon.Size();
  Vector2D<Scale_t> edge;
  Vector2D<Scale_t> to_point;
  // Anticlockwise edges bypass.
  for (size_t i = 0; i < polygon_size; ++i) {
    edge.Assign(polygon.points_[i], polygon.points_[(i + 1) % polygon_size]);
    to_point.Assign(polygon.points_[i], point);
    if (CrossProduct(edge, to_point) < 0) {
      return false;
    }
  }
  return true;
}

/// Return true is polygons have common point.
template<typename Scale_t>
bool HasIntersection(Polygon<Scale_t>& polygon1, Polygon<Scale_t>& polygon2) {
  Polygon<Scale_t> symmetric_polygon = GetSymmetricPolygon(polygon1);
  Polygon<Scale_t> sum_polygon =
      GetMinkowskiSumProduct(symmetric_polygon, polygon2);
  Point2D<Scale_t> zero(0, 0);
  return IsInPolygon(zero, sum_polygon);
}


#endif // INTERSECTION_HPP

