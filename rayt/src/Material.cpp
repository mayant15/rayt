#include "Material.h"
#include <glm/gtx/vector_angle.hpp>

constexpr double MAX_SCATTER_DIFFUSE = 5.0f;
constexpr double MAX_SCATTER_FUZZ = 1.0f;

double SchlickReflectance(double cos, double IOR) {
    auto r0 = (1 - IOR) / (1+ IOR);
    r0 = r0 * r0;
    return r0 + (1 - r0) * std::pow((1 - cos), 5);
}

bool Material::Scatter(const Ray& ray, const HitRecord& record, Colorf& attenuation, Ray& scattered)
{
    attenuation = Albedo;

    if (Type == MaterialType::METAL)
    {
        auto reflected = glm::reflect(ray.Direction(), record.Normal);
        reflected = RandomInSphere(record.Point + reflected, Roughness * MAX_SCATTER_FUZZ) - record.Point;
        scattered = Ray(record.Point, reflected);
        return glm::dot(reflected, record.Normal) > 0;
    }
    else if (Type == MaterialType::GLASS)
    {
        // Check if the ray is inside the surface
        bool outerSurface = (glm::dot(ray.Direction(), record.Normal) < 0);
        float refractingRatio = (outerSurface) ? 1.0f / IOR : IOR;
        auto normal = record.Normal * ((outerSurface) ? 1.0f : -1.0f);

        // Check if we have TIR
        double cosi = glm::dot(-1.0f * glm::normalize(ray.Direction()), glm::normalize(normal));
        double sinc = 1.0f / IOR;
        bool isTIR = (glm::acos(cosi) > glm::asin(sinc)) && !outerSurface && (Random() > 0.5);  // Fail to TIR sometimes

        glm::vec3 direction;
        if (isTIR || SchlickReflectance(cosi, refractingRatio) > Random())
        {
            direction = glm::reflect(ray.Direction(), normal);
        }
        else
        {
            direction = glm::refract(ray.Direction(), normal, refractingRatio);
        }

        scattered = Ray (record.Point, direction);
        return true;
    }
    else
    {
        auto scatterDir = RandomInSphere(record.Point + record.Normal, Roughness * MAX_SCATTER_DIFFUSE) - record.Point;
        scattered = Ray (record.Point, scatterDir);
        return true;
    }
}
