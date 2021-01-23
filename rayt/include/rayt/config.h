#pragma once

///////////////////////////////////////////////////////////////////////////////
// Renderer Settings
///////////////////////////////////////////////////////////////////////////////

#ifdef NDEBUG
#ifdef RAYT_ENABLE_VALIDATION_LAYERS
#undef RAYT_ENABLE_VALIDATION_LAYERS
#endif
#else

/** @brief Enable Vulkan validation layers
 *
 *  The default behaviour enables validation layers for debug builds and
 *  disables them for other builds.
 */
#define RAYT_ENABLE_VALIDATION_LAYERS

/** @brief Comma-separated list of required validation layer names */
#define RAYT_REQUIRED_VALIDATION_LAYERS \
    "VK_LAYER_KHRONOS_validation"
#endif


///////////////////////////////////////////////////////////////////////////////
// Window Settings
///////////////////////////////////////////////////////////////////////////////

/** @brief Window height for the client application */
#define RAYT_WINDOW_HEIGHT 720

/** @brief Window width for the client application */
#define RAYT_WINDOW_WIDTH  1024

/** @brief Window title for the client application */
#define RAYT_WINDOW_TITLE  "rayt"


///////////////////////////////////////////////////////////////////////////////
// Application Settings
///////////////////////////////////////////////////////////////////////////////

#define RAYT_VERSION_MAJOR 0
#define RAYT_VERSION_MINOR 0
#define RAYT_VERSION_PATCH 1
