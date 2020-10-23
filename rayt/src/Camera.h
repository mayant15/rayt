#pragma once

#include "Ray.h"
#include <cstdio>


class Camera
{
    glm::vec3 _origin;
    glm::vec3 _lowerLeftCorner;
    glm::vec3 _horizontal;
    glm::vec3 _vertical;

public:
    Camera()
    {
        auto aspectRatio = 16.0 / 9.0;
        auto viewportHeight = 2.0;
        auto viewportWidth = aspectRatio * viewportHeight;
        auto focalLength = 1.0;

        _origin = glm::vec3 {0.0f};
        _horizontal = glm::vec3 {viewportWidth, 0.0, 0.0};
        _vertical = glm::vec3 {0.0, viewportHeight, 0.0};
        _lowerLeftCorner = - (_horizontal / 2.0f) - (_vertical / 2.0f) - glm::vec3 {0, 0, focalLength} - _origin;

        printf("LowerLeftCorner: (%f, %f, %f)\n", _lowerLeftCorner.x, _lowerLeftCorner.y, _lowerLeftCorner.z);
    }

    /**
     * @brief Generate a ray that points towards (u, v) on the viewport
     * @param u Fraction of the viewport width
     * @param v Fraction of the viewport height
     * @return Ray from origin to (u * VIEWPORT_WIDTH, v * VIEWPORT_HEIGHT) on the viewport
    */
    Ray GetRayTo(float u, float v) const {
        return Ray(_origin, _lowerLeftCorner + u * _horizontal + v * _vertical);
    }
};
