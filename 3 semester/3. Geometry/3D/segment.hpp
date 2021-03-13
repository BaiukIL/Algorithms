#ifndef SEGMENT_HPP
#define SEGMENT_HPP

#include <cstdlib>
#include "point.hpp"


template <typename Scale_t>
struct Segment3D {

  Point3D<Scale_t> begin_;
  Point3D<Scale_t> end_;

  Segment3D()
      : begin_(Point3D<Scale_t>()),
        end_  (Point3D<Scale_t>()) {}

  Segment3D(Point3D<Scale_t> begin, Point3D<Scale_t> end)
      : begin_(begin),
        end_  (end) {}

  Segment3D(const Segment3D<Scale_t>& other)
      : begin_(other.begin_),
        end_  (other.end_) {}

  Segment3D& operator=(const Segment3D<Scale_t>& other) {
    begin_ = other.begin_;
    end_   = other.end_;
  }
};


#endif // SEGMENT_HPP

