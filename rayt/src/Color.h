#pragma once

#include "glm/glm.hpp"

typedef glm::vec<4, unsigned int, glm::defaultp> Colori;
typedef glm::vec<4, float, glm::defaultp> Colorf;

constexpr Colorf WHITE = Colorf {1.0f};

inline Colori ToIntegral(Colorf fractional)
{
    return glm::floor(255.0f * fractional);
}

inline Colorf ToFractional(Colori integral)
{
    return Colorf(integral) / 255.0f;
}

inline int ToRGBA(Colori iv)
{
    return (iv.a << 24) | (iv.b << 16) | (iv.g << 8) | (iv.r);
}

inline int ToRGBA(Colorf v)
{
    return ToRGBA(ToIntegral(v));
}
