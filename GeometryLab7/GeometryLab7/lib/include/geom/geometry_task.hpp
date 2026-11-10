#pragma once
#include "geom/geometry.hpp"
#include <vector>

namespace geom {

    class GeometryTask {
        std::vector<Point> points_;
        Point centroid_{};   // кеш: центр мас

        static Point computeCentroid(const std::vector<Point>& pts);
    public:
        GeometryTask() = default;
        explicit GeometryTask(std::vector<Point> pts);       // strong (кидає -> об'єкт не створюється)

        // Strong: перевірка всіх точок + обчислення центроїда у тимчасових об'єктах, потім swap
        void replacePoints(std::vector<Point> newPoints);
        // Strong: працює з копією вектора, фіксація через swap
        void addPoint(const Point& p);
        // No-throw
        void clear() noexcept;

        // No-throw
        std::size_t size() const noexcept { return points_.size(); }
        const Point& centroid() const noexcept { return centroid_; }
        const std::vector<Point>& points() const noexcept { return points_; }

        // Не змінюють стан -> strong; std::out_of_range при поганому індексі
        const Point& point(std::size_t i) const;
        double distance(std::size_t i, std::size_t j) const;
        double angleAtVertex(std::size_t i, std::size_t j, std::size_t k) const; // кут jik
        double triangleArea(std::size_t i, std::size_t j, std::size_t k) const;
        double triangleCircumradius(std::size_t i, std::size_t j, std::size_t k) const;
    };

} // namespace geom