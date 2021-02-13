#pragma once

#include <deque>
#include <vector>
#include <functional>

namespace rayt::detail
{
    class deletion_queue_t
    {
        std::deque<std::function<void()>> m_deletors;

    public:
        void push(std::function<void()>&& function)
        {
            m_deletors.emplace_back(function);
        }

        void flush()
        {
            for (auto it = m_deletors.rbegin(); it != m_deletors.rend(); ++it)
            {
                (*it)();
            }
            m_deletors.clear();
        }
    };
}
