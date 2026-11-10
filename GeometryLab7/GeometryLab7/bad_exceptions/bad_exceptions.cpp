#include <iostream>
#include <stdexcept>
#include <string>

int main() {
    try {
        try { throw 42; }
        catch (const std::exception& e) { std::cout << "std::exception caught: " << e.what() << '\n'; }
    }
    catch (...) { std::cout << "int slipped past std::exception, caught by catch(...)\n"; }

    try {
        try { throw std::string("text error"); }
        catch (const std::exception& e) { std::cout << "std::exception caught: " << e.what() << '\n'; }
    }
    catch (...) { std::cout << "std::string slipped past std::exception, caught by catch(...)\n"; }
}