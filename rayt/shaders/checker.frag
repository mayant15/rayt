#version 450

#define M_PI 3.1415926535897932384626433832795

layout (location = 0) in vec4 inPos;
layout (location = 0) out vec4 fragColor;

void main()
{
    if (sin(3 * M_PI * inPos.x) * sin(3 * M_PI * inPos.y) < 0)
    {
        fragColor = vec4 (0.1f);
    }
    else
    {
        fragColor = vec4 (0.9f);
    }
}
