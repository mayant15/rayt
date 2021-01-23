#include "application.h"
#include <rayt/logger.h>

application_t::application_t()
{
    LOG_INFO("Starting up");
    m_window_ptr = std::make_unique<rayt::window_t>();
    m_renderer_ptr = std::make_unique<rayt::renderer_t>();
}

void application_t::tick()
{
    while(!m_window_ptr->should_close())
    {
        m_window_ptr->poll_events();
    }
}

application_t::~application_t()
{
    LOG_INFO("Shutting down");
    m_renderer_ptr.reset();
    m_window_ptr.reset();
}
