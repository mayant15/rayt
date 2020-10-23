#pragma once

#include "glm/glm.hpp"

class Ray
{
    glm::vec3 _origin;
    glm::vec3 _direction;

public:
    Ray(
        glm::vec3 origin = {0.0f, 0.0f, 0.0f}, 
        glm::vec3 direction = {1.0f, 1.0f, 1.0f}
    ) : _origin(origin), _direction(direction) {}

    glm::vec3 At(double t) const
    {
        return _origin + _direction * (float) t;
    }

    glm::vec3 Direction() const
    {
        return _direction;
    }

    glm::vec3 Origin() const
    {
        return _origin;
    }
};
