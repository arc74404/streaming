#include "registry.hpp"

#include <stdexcept>

namespace stream::image::lin_impl
{

static void
registry_event_global(void* data,
                      uint32_t id,
                      uint32_t permissions,
                      const char* type,
                      uint32_t version,
                      const struct spa_dict* props)
{
    if (strcmp(type, PW_TYPE_INTERFACE_Node) != 0) return;

    if (props)
    {
        const char* media_class = spa_dict_lookup(props, PW_KEY_MEDIA_CLASS);

        if (media_class && strcmp(media_class, "Video/Source") == 0)
        {
            ScreenIdAndDescription info;
            info.name = spa_dict_lookup(props, PW_KEY_NODE_NAME);
            info.id   = id;

            reinterpret_cast<Registry*>(data)->pushInfo(std::move(info));
        }
    }
}

void
Registry::pushInfo(ScreenIdAndDescription&& data)
{
    m_screens_info.emplace_back(std::move(data));
}

Registry::Registry(pw_core* core)
{
    m_registry = pw_core_get_registry(core, PW_VERSION_REGISTRY, 0);

    m_events.version = PW_VERSION_REGISTRY_EVENTS;
    m_events.global  = registry_event_global;

    pw_registry_add_listener(m_registry, &m_registry_listener, &m_events, this);
}

Registry::~Registry()
{
    spa_hook_remove(&m_registry_listener);
    pw_proxy_destroy((struct pw_proxy*)m_registry);
}
} // namespace stream::image::lin_impl
