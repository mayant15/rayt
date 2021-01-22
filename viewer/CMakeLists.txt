find_package(glfw3 CONFIG REQUIRED)

set(VIEWER_INCLUDE_DIRS
    include
    external
    src
)

set(VIEWER_SOURCES
    src/main.cpp
    src/application.cpp
)

add_executable(viewer ${VIEWER_SOURCES})
add_dependencies(viewer rayt)
target_include_directories(viewer PUBLIC ${VIEWER_INCLUDE_DIRS})
target_link_libraries(viewer PUBLIC rayt glfw)
