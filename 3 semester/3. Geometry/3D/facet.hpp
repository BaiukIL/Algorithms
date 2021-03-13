#ifndef FACET_HPP
#define FACET_HPP

#include "point.hpp"


template <typename Scale_t>
struct Facet {

    Point3D<Scale_t>* points_[3];

    Facet(Point3D<Scale_t>* first,
          Point3D<Scale_t>* second,
          Point3D<Scale_t>* third) {
      points_[0] = first;
      points_[1] = second;
      points_[2] = third;
    }

    Facet() {
      points_[0] = nullptr;
      points_[1] = nullptr;
      points_[2] = nullptr;
    }
};


#endif // FACET_HPP

