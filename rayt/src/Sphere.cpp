#include "Sphere.h"

bool Sphere::Hit(const Ray& ray, double tMin, double tMax, HitRecord& record) const
{
    auto AC = ray.Origin() - Center;
    double A = glm::dot(ray.Direction(), ray.Direction());
    double H = glm::dot(AC, ray.Direction());
    double C = glm::dot(AC, AC) - Radius * Radius;

    auto discriminant = H * H - A * C;
    if (discriminant > 0) {
        // Two intersections
        auto alpha = (-H - std::sqrt(discriminant)) / A;
        auto beta = (-H + std::sqrt(discriminant)) / A;

        double t = 0.0;
        if (alpha < tMax && alpha > tMin) {
            t = alpha;
        }
        else if (beta < tMax && beta > tMin)
        {
            t = beta;
        }
        else
        {
            return false;
        }

        record.t = t;
        record.Point = ray.At(t);
        record.Normal = glm::normalize(record.Point - Center);
        record.Mat = Mat;
        return true;
    }

    return false;
}
