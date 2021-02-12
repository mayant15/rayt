find_package(Vulkan REQUIRED)
find_package(Eigen3 CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)

set(RAYT_INCLUDE_DIRS
    include
    external
    src
)

set(RAYT_SOURCES
    external/stb_image_write.cpp
    external/bootstrap.cpp

    src/renderer.cpp
    src/window.cpp
)

add_library(rayt SHARED ${RAYT_SOURCES})
target_include_directories(rayt PUBLIC ${RAYT_INCLUDE_DIRS})
target_link_libraries(rayt PUBLIC Eigen3::Eigen spdlog::spdlog Vulkan::Vulkan glfw)
