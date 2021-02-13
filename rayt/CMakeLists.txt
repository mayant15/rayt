find_package(Vulkan REQUIRED)
find_package(Eigen3 CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)

# Build shaders

find_program(GLSL_VALIDATOR glslangValidator HINTS /usr/bin /usr/local/bin $ENV{VULKAN_SDK}/Bin/ $ENV{VULKAN_SDK}/Bin32/)

set(GLSL_SOURCE_FILES
    shaders/triangle.vert
    shaders/triangle.frag
    )

foreach (GLSL ${GLSL_SOURCE_FILES})
    get_filename_component(FILE_NAME ${GLSL} NAME)
    get_filename_component(GLSL_ABSOLUTE ${GLSL} ABSOLUTE)
    set(SPIRV "${PROJECT_BINARY_DIR}/shaders/${FILE_NAME}.spv")
    add_custom_command(
        OUTPUT ${SPIRV}
        COMMAND ${CMAKE_COMMAND} -E make_directory "${PROJECT_BINARY_DIR}/shaders/"
        COMMAND ${GLSL_VALIDATOR} -V ${GLSL_ABSOLUTE} -o ${SPIRV}
        DEPENDS ${GLSL}
    )
    list(APPEND SPIRV_BINARY_FILES ${SPIRV})
endforeach (GLSL)

add_custom_target(
    shaders
    DEPENDS ${SPIRV_BINARY_FILES}
)

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
    src/pipelines.cpp
)

add_library(rayt SHARED ${RAYT_SOURCES})
add_dependencies(rayt shaders)
target_include_directories(rayt PUBLIC ${RAYT_INCLUDE_DIRS})
target_link_libraries(rayt
    PUBLIC Eigen3::Eigen spdlog::spdlog Vulkan::Vulkan glfw
    )

# Copy built shaders to rayt dll location

add_custom_command(TARGET rayt POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory "$<TARGET_FILE_DIR:rayt>/shaders/"
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${PROJECT_BINARY_DIR}/shaders"
    "$<TARGET_FILE_DIR:rayt>/shaders"
    )
