#pragma once
#include <cstddef>

namespace geom {

    struct Point {
        double x = 0.0;
        double y = 0.0;
        bool operator==(const Point&) const = default;
    };

    void validatePoint(const Point& p, std::size_t index);   // кидає InvalidCoordinateException
    double distance(const Point& a, const Point& b);          // ResultOverflowException
    double angleBetween(const Point& u, const Point& v);      // вектори; [0, pi]
    double triangleArea(const Point& a, const Point& b, const Point& c);
    double triangleCircumradius(const Point& a, const Point& b, const Point& c);

} // namespace geom