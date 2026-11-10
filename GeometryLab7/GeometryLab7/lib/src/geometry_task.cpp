#include "geom/geometry_task.hpp"
#include "geom/exceptions.hpp"
#include <cmath>
#include <stdexcept>

namespace geom {

    Point GeometryTask::computeCentroid(const std::vector<Point>& pts) {
        if (pts.empty()) return { 0.0, 0.0 };
        double sx = 0.0, sy = 0.0;
        for (const auto& p : pts) { sx += p.x; sy += p.y; }
        if (!std::isfinite(sx) || !std::isfinite(sy))
            throw ResultOverflowException("centroid");
        return { sx / pts.size(), sy / pts.size() };
    }

    GeometryTask::GeometryTask(std::vector<Point> pts) {
        replacePoints(std::move(pts));
    }

    void GeometryTask::replacePoints(std::vector<Point> newPoints) {
        // 1. prepare (може кидати)
        for (std::size_t i = 0; i < newPoints.size(); ++i) validatePoint(newPoints[i], i);
        Point c = computeCentroid(newPoints);
        // 2. commit (noexcept)
        points_.swap(newPoints);
        centroid_ = c;
    }

    void GeometryTask::addPoint(const Point& p) {
        validatePoint(p, points_.size());
        std::vector<Point> tmp = points_;       // може кидати bad_alloc, але points_ не змінено
        tmp.push_back(p);
        Point c = computeCentroid(tmp);
        points_.swap(tmp);
        centroid_ = c;
    }

    void GeometryTask::clear() noexcept {
        points_.clear();
        centroid_ = { 0.0, 0.0 };
    }

    const Point& GeometryTask::point(std::size_t i) const {
        if (i >= points_.size()) throw std::out_of_range("Invalid point index");
        return points_[i];
    }

    double GeometryTask::distance(std::size_t i, std::size_t j) const {
        return geom::distance(point(i), point(j));
    }

    double GeometryTask::angleAtVertex(std::size_t i, std::size_t j, std::size_t k) const {
        const Point& o = point(i); const Point& a = point(j); const Point& b = point(k);
        return angleBetween({ a.x - o.x, a.y - o.y }, { b.x - o.x, b.y - o.y });
    }

    double GeometryTask::triangleArea(std::size_t i, std::size_t j, std::size_t k) const {
        return geom::triangleArea(point(i), point(j), point(k));
    }

    double GeometryTask::triangleCircumradius(std::size_t i, std::size_t j, std::size_t k) const {
        return geom::triangleCircumradius(point(i), point(j), point(k));
    }

} // namespace geom