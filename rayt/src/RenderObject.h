#pragma once

#include "Ray.h"
#include "Util.h"

class Material;
struct HitRecord
{
    glm::vec3 Point;
    glm::vec3 Normal;
    SRef<Material> Mat;
    double t;
};

class RenderObject
{
public:
    virtual bool Hit(const Ray& ray, double tMin, double tMax, HitRecord& record) const = 0;
};
