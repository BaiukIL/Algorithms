#ifndef SEGM_DISTANCE_HPP
#define SEGM_DISTANCE_HPP

#include <cmath>
#include "point.hpp"
#include "segment.hpp"

#ifndef PRECISION
#define PRECISION 1e-10
#endif


//========================= POINT -- POINT =====================================
template <typename Scale_t>
double GetDistance(const Point3D<Scale_t>& point1,
                   const Point3D<Scale_t>& point2) {
  Scale_t x_len = point1.x_ - point2.x_;
  Scale_t y_len = point1.y_ - point2.y_;
  Scale_t z_len = point1.z_ - point2.z_;
  return sqrt(x_len*x_len + y_len*y_len + z_len*z_len);
}

//========================= POINT -- SEGMENT ===================================
// Use ternary search.
template <typename Scale_t>
double GetDistance(const Point3D<Scale_t>& point, Segment3D<Scale_t> segm) {
  Point3D<Scale_t> begin_third;
  Point3D<Scale_t> end_third;

  while (GetDistance(segm.begin_, segm.end_) > PRECISION) {
    begin_third = (segm.begin_ * 2 + segm.end_) / 3;
    end_third   = (segm.begin_ + segm.end_ * 2) / 3;
    if (GetDistance(begin_third, point) < GetDistance(end_third, point)) {
      segm.end_ = end_third;
    } else {
      segm.begin_ = begin_third;
    }
  }
  return GetDistance((segm.begin_ + segm.end_) / 2, point);
}

//========================= SEGMENT -- SEGMENT =================================
// Use ternary search.
template <typename Scale_t>
double GetDistance(Segment3D<Scale_t> segm1, const Segment3D<Scale_t>& segm2) {
  Point3D<Scale_t> begin_third;
  Point3D<Scale_t> end_third;

  while (GetDistance(segm1.begin_, segm1.end_) > PRECISION) {
    begin_third = (segm1.begin_ * 2 + segm1.end_) / 3;
    end_third   = (segm1.begin_ + segm1.end_ * 2) / 3;
    if (GetDistance(begin_third, segm2) < GetDistance(end_third, segm2)) {
      segm1.end_ = end_third;
    } else {
      segm1.begin_ = begin_third;
    }
  }
  return GetDistance((segm1.begin_ + segm1.end_) / 2, segm2);
}


#endif // SEGM_DISTANCE_HPP

