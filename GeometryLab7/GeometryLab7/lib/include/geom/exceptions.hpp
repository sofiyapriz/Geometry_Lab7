#pragma once
#include <cstddef>
#include <stdexcept>
#include <string>

namespace geom {

    class GeometryException : public std::runtime_error {
    public:
        explicit GeometryException(const std::string& msg) : std::runtime_error(msg) {}
    };

    // NaN/Inf у координатах
    class InvalidCoordinateException : public GeometryException {
        std::size_t index_; std::string coord_; double value_;
    public:
        InvalidCoordinateException(std::size_t index, std::string coord, double value)
            : GeometryException("Non-finite coordinate '" + coord + "' of point #" + std::to_string(index)),
            index_(index), coord_(std::move(coord)), value_(value) {
        }
        std::size_t index() const noexcept { return index_; }
        const std::string& coord() const noexcept { return coord_; }
        double value() const noexcept { return value_; }
    };

    // нульовий вектор, вироджений трикутник
    class DegenerateShapeException : public GeometryException {
        std::string operation_;
    public:
        DegenerateShapeException(std::string operation, const std::string& reason)
            : GeometryException(operation + ": " + reason), operation_(std::move(operation)) {
        }
        const std::string& operation() const noexcept { return operation_; }
    };

    // переповнення результату
    class ResultOverflowException : public GeometryException {
        std::string operation_;
    public:
        explicit ResultOverflowException(std::string operation)
            : GeometryException(operation + ": result is not finite (overflow)"),
            operation_(std::move(operation)) {
        }
        const std::string& operation() const noexcept { return operation_; }
    };

} // namespace geom