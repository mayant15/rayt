#pragma once

#include <random>
#include <memory>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>


constexpr double PI = 3.1415926535897932385;
constexpr double INF = std::numeric_limits<double>::infinity();

inline double DegToRad(double degrees) {
    return degrees * PI / 180.0;
}

template<typename T>
using SRef = std::shared_ptr<T>;

/**
 * @brief Returns a random number between 0 and 1
 * @return double
*/
inline double Random() {
    static std::uniform_real_distribution<double> distribution(0.0, 1.0);
    static std::mt19937 generator;
    return distribution(generator);
}

inline glm::vec3 RandomInSphere(glm::vec3 center, double radius)
{
    double theta = Random() * 2 * PI;
    double phi = Random() * PI;
    float length = Random() * radius;
    glm::vec3 direction { glm::cos(theta) * glm::cos(phi), glm::cos(theta) * glm::sin(phi), glm::sin(theta) };
    return length * direction + center;
}

inline double Clamp(double x, double min, double max) {
    if (x < min) return min;
    if (x > max) return max;
    return x;
}
