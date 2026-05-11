#pragma once

#include <pipewire/pipewire.h>
#include <stdexcept>

namespace stream::image::lin_impl
{
template <typename Events>
class Listener final
{
public:
    template <typename Creater, typename... Args>
    Listener(Creater&& creater, const Events& events, Args&&... args)
        : m_events(events)
    {
        bool res = creater(&m_listener, &m_events, args...);

        if (!res)
        {
            throw std::runtime_error("Failed create Listener");
        }
    }
    Listener(const Listener&) = delete;
    Listener(Listener&&)      = delete;

    Listener& operator=(const Listener&) = delete;
    Listener& operator=(Listener&&)      = delete;

    ~Listener() noexcept
    {
        spa_hook_remove(&m_listener);
    }

private:
    Events m_events;
    spa_hook m_listener;
};
} // namespace stream::image::lin_impl
