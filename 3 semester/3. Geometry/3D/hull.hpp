#ifndef HULL_HPP
#define HULL_HPP

#include "point.hpp"
#include "facet.hpp"
#include <cassert>
#include <limits>
#include <vector>
#include <unordered_set>

/* * It is Chan's 3D Divide-and-Conquer Convex Hull Algorithm implementation.  * See original paper at http://tmc.web.engr.illinois.edu/ch3d/ch3d.pdf *
 * I admit that most part of implementation consides exactly with Chan's one,
 * for I find author's implementation too neat to improve (with the same
 * memory and speed limits) and thoughtfull enough to miss some important
 * details while writing on my own.
 *
 * However, there is a difference from original implementation: it can handle
 * simultaneous events. See BuildLowerHull function code for more information.
 */

/// List-like point. Help entity for Hull.
struct HullPoint {

    Point3D<double>* point_;
    HullPoint* prev_;
    HullPoint* next_;

    HullPoint() : point_(nullptr),
                  prev_ (nullptr),
                  next_ (nullptr) {}

    HullPoint(Point3D<double>& point) : point_(&point),
                                        prev_ (nullptr),
                                        next_ (nullptr) {}

    void Act() {
      // Inserion.
      if (prev_->next_ != this) { prev_->next_ = next_->prev_ = this; }
      // Deletion.
      else { prev_->next_ = next_; next_->prev_ = prev_; }
    }
};


class Hull {

    static constexpr double INF = std::numeric_limits<double>::max();
    static Point3D<double> nil_point;
    static HullPoint nil;
    // Note that we cannot replace NIL with nullptr, since there are cases
    // we directly access point coordinates.
    static HullPoint* NIL;

  private:
    /// Pointers to convex hull points.
    std::unordered_set<Point3D<double>*> points_;
    /// Array of hull points.
    std::vector<HullPoint> hull_points_;

    // **************************** Additional ********************************
    bool has_projection_;
    /// Pointers to 2D convex hull points.
    std::unordered_set<Point3D<double>*> projection_points_;

    bool has_facets_;
    /// Facets (triples of points) of convex hulls.
    std::vector<Facet<double>> facets_;

  public:

    Hull(std::vector<Point3D<double>>& points, // Points are not const!
         bool build_facets,
         bool build_projection);

    /// Return number of vertices in convex hull.
    size_t Size() const { return points_.size(); }
    /// Return convex hull mass center.
    Point3D<double> GetCenter() const {
      Point3D<double> center;
      for (Point3D<double>* point : points_) { center += *point; }
      return center /= Size();
    }

    const std::unordered_set<Point3D<double>*>& GetPoints() const {
      return points_;
    }

    // **************************** Additional ********************************
    const std::vector<Facet<double>>& GetFacets() const {
      if (!has_facets_) assert("Hull does not have facets access");
      return facets_;
    }

    const std::unordered_set<Point3D<double>*>& GetProjectionPoints() const {
      if (!has_projection_) assert("Hull does not have projection access");
      return projection_points_;
    }

  private:
    /// Return [p, q] and [p, r] vectors cross product.
    double GetTurn(HullPoint* p, HullPoint* q, HullPoint* r);
    /// Return time (t handled from y` = z = t*y) when p, q and r lies
    /// on same line.
    double GetTime(HullPoint* p, HullPoint* q, HullPoint* r);
    /// List mergesort.
    /// Sort points array by x coordinate and return list of Point-s
    /// (first element of this list).
    HullPoint* Sort(HullPoint* points, size_t n);
    /// Main function.
    /// Build lower convex hull.
    void BuildLowerHull(HullPoint* list, size_t n,
                        HullPoint** events,
                        HullPoint** tmp);
    void AddPoint(HullPoint* point);

    // ****************************** Facet ******************************
    /// Add facet and appropriate points to the hull.
    void AddFacet(HullPoint* point);
    /// Build lower and upper convex hulls.
    void BuildFullHull(std::vector<Point3D<double>>& points);

    // *************************** Projection ****************************
    void BuildHalfProjection(HullPoint* list);
};


#endif // HULL_HPP

