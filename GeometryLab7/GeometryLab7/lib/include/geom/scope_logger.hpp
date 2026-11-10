#pragma once
#include <iostream>
#include <string>

namespace geom {

    class ScopeLogger {
        std::string name_;
    public:
        explicit ScopeLogger(std::string name) : name_(std::move(name)) {
            std::cout << "Created: " << name_ << '\n';
        }
        ~ScopeLogger() { std::cout << "Destroyed: " << name_ << '\n'; }
        ScopeLogger(const ScopeLogger&) = delete;
        ScopeLogger& operator=(const ScopeLogger&) = delete;
    };

} // namespace geom