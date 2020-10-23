#pragma once

#include "RenderObject.h"
#include "Color.h"

enum class MaterialType
{
    DIFFUSE,
    METAL,
    GLASS
};

class Material
{
public:
    Colorf Albedo;
    MaterialType Type;
    double Roughness = 0.5;
    double IOR = 1;

    bool Scatter(const Ray& ray, const HitRecord& record, Colorf& attenuation, Ray& scattered);
};
