# rayt

rayt is a multithreaded Vulkan rendering engine acting as a scratchpad for [Lotus](https://github.com/mayant15/lotus/).
This repository contains proof-of-concept implementations for experimental techniques.

![rtweekend](https://i.imgur.com/eIuheMU.png)

## Dependencies

rayt depends on these libraries which are **NOT** provided with the source.

- [GLFW](https://github.com/glfw/glfw)
- [VulkanSDK](https://vulkan.lunarg.com/sdk/home)
- [Eigen](https://eigen.tuxfamily.org/index.php?title=Main_Page)
- [spdlog](https://github.com/gabime/spdlog)

## About

My goal with Lotus is to provide high quality visuals even with older hardware. Inspired by Crytek's [Neon Noir demo](https://www.cryengine.com/news/view/crytek-releases-neon-noir-a-real-time-ray-tracing-demonstration-for-cryengine), 
I decided to experiment with bringing ray tracing support to non-RTX cards. This repository is meant to act as a sandbox where I implement algorithms found by surveying published research, and 
the ones that are successful will eventually make their way into Lotus.
