#pragma once

#include <iostream>
#include <vector>
#include <string>

#include "stb_image_write.h"
#include "Color.h"

constexpr unsigned int RT_MONO = 1;
constexpr unsigned int RT_MONOA = 2;
constexpr unsigned int RT_RGB = 3;
constexpr unsigned int RT_RGBA = 4;

inline void WriteImage(const std::string& path, unsigned int width, unsigned int height, const std::vector<Colorf>& data)
{
    std::vector<int> idata(height * width);
    for (size_t i = 0; i < height; i++)
    {
        for (size_t j = 0; j < width; j++)
        {
            int pixel = j + i * width;
            idata[pixel] = ToRGBA(data[pixel]);
        }
    }

    stbi_write_png("out.png", width, height, RT_RGBA, idata.data(), width * sizeof (int));


}
