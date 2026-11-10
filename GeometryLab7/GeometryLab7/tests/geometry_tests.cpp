#include <gtest/gtest.h>
#include <cmath>
#include <limits>
#include "geom/exceptions.hpp"
#include "geom/geometry_task.hpp"

using namespace geom;
using Pts = std::vector<Point>;
const double NaN = std::numeric_limits<double>::quiet_NaN();
const double Inf = std::numeric_limits<double>::infinity();
const double Pi = 3.14159265358979323846;

// ---------- основні операції ----------
TEST(Geometry, Distance345) {
    EXPECT_NEAR(distance({ 0, 0 }, { 3, 4 }), 5.0, 1e-12);
}
TEST(Geometry, AngleRightAngle) {
    EXPECT_NEAR(angleBetween({ 1, 0 }, { 0, 1 }), Pi / 2, 1e-12);
}
TEST(Geometry, TriangleArea) {
    EXPECT_NEAR(triangleArea({ 0, 0 }, { 4, 0 }, { 0, 3 }), 6.0, 1e-12);
}
TEST(Geometry, Circumradius345) {
    EXPECT_NEAR(triangleCircumradius({ 0, 0 }, { 3, 0 }, { 0, 4 }), 2.5, 1e-12);
}

// ---------- сценарії помилок ----------
TEST(Errors, NaNCoordinate) {
    EXPECT_THROW(GeometryTask(Pts{ {0, 0}, {NaN, 1} }), InvalidCoordinateException);
}
TEST(Errors, InfCoordinate) {
    EXPECT_THROW(GeometryTask(Pts{ {Inf, 1} }), InvalidCoordinateException);
}
TEST(Errors, ExceptionCarriesDiagnostics) {
    try { GeometryTask t(Pts{ {0, 0}, {1, Inf} }); FAIL() << "no throw"; }
    catch (const InvalidCoordinateException& e) {
        EXPECT_EQ(e.index(), 1u);
        EXPECT_EQ(e.coord(), "y");
    }
}
TEST(Errors, ZeroVectorAngle) {
    EXPECT_THROW(angleBetween({ 0, 0 }, { 1, 1 }), DegenerateShapeException);
}
TEST(Errors, BadIndex) {
    GeometryTask t(Pts{ {0, 0}, {1, 1} });
    EXPECT_THROW(t.point(2), std::out_of_range);
    EXPECT_THROW(t.distance(0, 5), std::out_of_range);
}
TEST(Errors, DegenerateTriangleCircumradius) {
    EXPECT_THROW(triangleCircumradius({ 0, 0 }, { 1, 1 }, { 2, 2 }), DegenerateShapeException);
}
TEST(Errors, DistanceOverflow) {
    EXPECT_THROW(distance({ -1e308, 0 }, { 1e308, 0 }), ResultOverflowException);
}
TEST(Errors, CatchableAsBase) {
    EXPECT_THROW(distance({ -1e308, 0 }, { 1e308, 0 }), GeometryException);
    EXPECT_THROW(distance({ -1e308, 0 }, { 1e308, 0 }), std::runtime_error);
}

// ---------- граничні значення ----------
TEST(Boundary, ZeroDistance) { EXPECT_DOUBLE_EQ(distance({ 1, 1 }, { 1, 1 }), 0.0); }
TEST(Boundary, EmptySetAllowed) { GeometryTask t; EXPECT_EQ(t.size(), 0u); }
TEST(Boundary, LastIndexOkSizeBad) {
    GeometryTask t(Pts{ {0, 0}, {1, 1} });
    EXPECT_NO_THROW(t.point(1));
    EXPECT_THROW(t.point(2), std::out_of_range);
}
TEST(Boundary, OppositeVectorsAnglePi) {
    EXPECT_NEAR(angleBetween({ 1, 0 }, { -1, 0 }), Pi, 1e-12);
}

// ---------- інваріанти ----------
TEST(Invariants, CentroidMatchesPoints) {
    GeometryTask t(Pts{ {0, 0}, {2, 4} });
    EXPECT_NEAR(t.centroid().x, 1.0, 1e-12);
    EXPECT_NEAR(t.centroid().y, 2.0, 1e-12);
    t.addPoint({ 4, 2 });
    EXPECT_NEAR(t.centroid().x, 2.0, 1e-12);
    EXPECT_NEAR(t.centroid().y, 2.0, 1e-12);
}
TEST(Invariants, ClearResetsAll) {
    GeometryTask t(Pts{ {1, 1} });
    t.clear();
    EXPECT_EQ(t.size(), 0u);
    EXPECT_EQ(t.centroid(), (Point{ 0, 0 }));
}

// ---------- сильна гарантія ----------
TEST(StrongGuarantee, ReplaceWithNaNKeepsState) {
    GeometryTask t(Pts{ {0, 0}, {2, 2} });
    EXPECT_THROW(t.replacePoints(Pts{ {1, 1}, {NaN, 0}, {5, 5} }), InvalidCoordinateException);
    ASSERT_EQ(t.size(), 2u);
    EXPECT_EQ(t.point(0), (Point{ 0, 0 }));
    EXPECT_EQ(t.point(1), (Point{ 2, 2 }));
    EXPECT_EQ(t.centroid(), (Point{ 1, 1 }));
}
TEST(StrongGuarantee, ReplaceWithCentroidOverflowKeepsState) {
    GeometryTask t(Pts{ {0, 0}, {2, 2} });
    // усі координати скінченні, але сума переповнюється -> виняток на етапі prepare
    EXPECT_THROW(t.replacePoints(Pts{ {1e308, 0}, {1e308, 0} }), ResultOverflowException);
    ASSERT_EQ(t.size(), 2u);
    EXPECT_EQ(t.point(1), (Point{ 2, 2 }));
    EXPECT_EQ(t.centroid(), (Point{ 1, 1 }));
}
TEST(StrongGuarantee, AddInvalidPointKeepsState) {
    GeometryTask t(Pts{ {0, 0}, {2, 2} });
    EXPECT_THROW(t.addPoint({ 0, Inf }), InvalidCoordinateException);
    EXPECT_EQ(t.size(), 2u);
    EXPECT_EQ(t.centroid(), (Point{ 1, 1 }));
}
TEST(StrongGuarantee, SuccessfulReplaceWorks) {
    GeometryTask t(Pts{ {0, 0} });
    t.replacePoints(Pts{ {2, 2}, {4, 4} });
    EXPECT_EQ(t.size(), 2u);
    EXPECT_EQ(t.centroid(), (Point{ 3, 3 }));
}