#include "geom/geometry.hpp"
#include "geom/exceptions.hpp"
#include <cmath>

namespace geom {

    namespace {
        double checked(double v, const char* op) {
            if (!std::isfinite(v)) throw ResultOverflowException(op);
            return v;
        }
    }

    void validatePoint(const Point& p, std::size_t index) {
        if (!std::isfinite(p.x)) throw InvalidCoordinateException(index, "x", p.x);
        if (!std::isfinite(p.y)) throw InvalidCoordinateException(index, "y", p.y);
    }

    double distance(const Point& a, const Point& b) {
        return checked(std::hypot(b.x - a.x, b.y - a.y), "distance");
    }

    double angleBetween(const Point& u, const Point& v) {
        if ((u.x == 0 && u.y == 0) || (v.x == 0 && v.y == 0))
            throw DegenerateShapeException("angleBetween", "zero vector");
        double cross = checked(u.x * v.y - u.y * v.x, "angleBetween");
        double dot = checked(u.x * v.x + u.y * v.y, "angleBetween");
        return std::atan2(std::fabs(cross), dot);
    }

    double triangleArea(const Point& a, const Point& b, const Point& c) {
        double cross = (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x);
        return checked(0.5 * std::fabs(cross), "triangleArea");
    }

    double triangleCircumradius(const Point& a, const Point& b, const Point& c) {
        double cross = (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x);
        if (!std::isfinite(cross)) throw ResultOverflowException("triangleCircumradius");
        if (std::fabs(cross) < 1e-12)
            throw DegenerateShapeException("triangleCircumradius", "degenerate triangle");
        // R = |AB|*|BC|*|CA| / (4S) = |AB|*|BC|*|CA| / (2*|cross|)
        return checked(distance(a, b) * distance(b, c) * distance(c, a) / (2.0 * std::fabs(cross)),
            "triangleCircumradius");
    }

} // namespace geom