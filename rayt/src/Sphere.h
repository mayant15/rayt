#pragma once

#include "RenderObject.h"

class Sphere : public RenderObject
{
public:
    glm::vec3 Center;
    double Radius;
    SRef<Material> Mat;


    Sphere(glm::vec3 center, double radius, SRef<Material> material) : Center(center), Radius(radius), Mat(material) {}

    virtual bool Hit(const Ray& ray, double tMin, double tMax, HitRecord& record) const override;
};
