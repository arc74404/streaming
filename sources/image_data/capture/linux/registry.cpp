#include "registry.hpp"

#include <functional>
#include <iostream>
#include <stdexcept>

void
stream::image::lin_impl::registry_event_global(void* data,
                                               uint32_t id,
                                               uint32_t permissions,
                                               const char* type,
                                               uint32_t version,
                                               const spa_dict* props)
{
    auto* self = reinterpret_cast<Registry*>(data);
    if (strcmp(type, PW_TYPE_INTERFACE_Node) != 0) return;

    if (props)
    {
        const char* media_class = spa_dict_lookup(props, PW_KEY_MEDIA_CLASS);
        if (media_class)
        {
            if (media_class)
            {
                ScreenIdAndDescription info;
                const char* name = spa_dict_lookup(props, PW_KEY_NODE_NAME);
                if (name)
                {
                    info.name = name;
                }
                info.id = id;

                self->m_screens_info.emplace_back(std::move(info));
            }
        }
    }
}

namespace stream::image::lin_impl
{

Registry::Registry(pw_core*& core, pw_thread_loop* loop) : m_syncer(loop)
{
    m_registry = pw_core_get_registry(core, PW_VERSION_REGISTRY, 0);

    m_register_events.version = PW_VERSION_REGISTRY_EVENTS;
    m_register_events.global  = registry_event_global;

    m_registry_listener.emplace(
        [this](spa_hook* hook, const pw_registry_events* events, void* data)
        {
            pw_registry_add_listener(m_registry, hook, events, data);
            return true;
        },
        m_register_events, this);
}

Registry::~Registry()
{
    if (m_registry)
    {
        m_syncer.call(pw_proxy_destroy, (pw_proxy*)m_registry);
    }
}

const std::vector<ScreenIdAndDescription>&
Registry::getIdsAndDescriptions()
{
    return m_screens_info;
}
} // namespace stream::image::lin_impl
