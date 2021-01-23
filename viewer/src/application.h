#pragma once

#include <rayt/window.h>
#include <rayt/renderer.h>

#include <memory>

class application_t
{
public:
    application_t();
    ~application_t();

    void tick();

private:
    std::unique_ptr<rayt::window_t> m_window_ptr;
    std::unique_ptr<rayt::renderer_t> m_renderer_ptr;
};
