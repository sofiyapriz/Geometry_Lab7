#include "geom/exceptions.hpp"
#include "geom/geometry_task.hpp"
#include "geom/scope_logger.hpp"
#include <iostream>

using namespace geom;

void level3(const GeometryTask& t) {
    ScopeLogger log("level3");
    std::cout << t.angleAtVertex(0, 1, 2) << '\n';   // нульовий вектор -> виняток
}

void level2(const GeometryTask& t) {
    ScopeLogger log("level2");
    try {
        level3(t);
    }
    catch (const GeometryException& e) {
        std::cerr << "[level2] diagnostics: " << e.what() << '\n';
        throw;                                       // саме throw;, не throw e;
    }
}

void level1(const GeometryTask& t) {
    ScopeLogger log("level1");
    level2(t);
    std::cout << "never printed\n";
}

int main() {
    ScopeLogger mainLog("main");
    try {
        GeometryTask t(std::vector<Point>{{0, 0}, { 0, 0 }, { 1, 1 }});
        level1(t);
    }
    catch (const DegenerateShapeException& e) { std::cout << "[main] degenerate: " << e.what() << '\n'; }
    catch (const GeometryException& e) { std::cout << "[main] geometry: " << e.what() << '\n'; }
    catch (const std::exception& e) { std::cout << "[main] std: " << e.what() << '\n'; }
    std::cout << "Program continues\n";
}