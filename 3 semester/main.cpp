#include <cstdio>
#include <algorithm>
#include "3D/hull.hpp"
#include "3D/vector.hpp"


struct FacetID {

    size_t points_id_[3];
    FacetID() {}
    /// Construct face's points indexes, sorted in counterclockwise direction
    /// from outer normal to the face.
    FacetID(const Point3D<double>* points, const Facet<double>& facet,
            const Point3D<double>& center) {
      for (size_t i = 0; i < 3; ++i) {
        points_id_[i] = facet.points_[i] - points;
      }
      // Sort indexes.
      for (size_t i = 0; i < 2; ++i) {
        for (size_t j = 0; j < 2 - i; ++j) {
          if (points_id_[j] > points_id_[j + 1]) {
            std::swap(points_id_[j], points_id_[j + 1]);
          }
        }
      }
      Vector3D<double> to_center(points[points_id_[0]], center);
      Vector3D<double> edge1(points[points_id_[0]], points[points_id_[1]]);
      Vector3D<double> edge2(points[points_id_[0]], points[points_id_[2]]);
      if (DotProduct(CrossProduct(edge1, edge2), to_center) > 0) {
        std::swap(points_id_[1], points_id_[2]);
      }
    }
};


int main() {
  size_t tests_num = 0;
  size_t points_num = 0;
  scanf("%lu", &tests_num);

  for (size_t test = 0; test < tests_num; ++test) {
    scanf("%lu", &points_num);

    std::vector<Point3D<double>> points(points_num);
    for (size_t i = 0; i < points_num; ++i) {
      scanf("%lf %lf %lf", &points[i].x_, &points[i].y_, &points[i].z_);
    }
    printf("\n");

    Hull hull(points, true, true);
    std::vector<Facet<double>> facets = hull.GetFacets();
    size_t facets_num = facets.size();
    std::vector<FacetID> facets_id(facets_num);

    // Create facets' points indexes array.
    Point3D<double> center = hull.GetCenter();
    for (size_t i = 0; i < facets_num; ++i) {
      facets_id[i] = FacetID(points.data(), facets[i], center);
    }

    // Sort facets lexicographically.
    std::sort(facets_id.begin(), facets_id.end(),
        [](FacetID& facet1, FacetID& facet2)->bool
        {
          return std::lexicographical_compare(facet1.points_id_,
                                              facet1.points_id_ + 3,
                                              facet2.points_id_,
                                              facet2.points_id_ + 3);
        });

    printf("%lu\n", facets_num);
    for (auto& facet_id : facets_id) {
      printf("3 %ld %ld %ld\n", facet_id.points_id_[0],
                                facet_id.points_id_[1],
                                facet_id.points_id_[2]);
    }
  }

  return 0;
}

