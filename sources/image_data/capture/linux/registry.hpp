#pragma once

#include <pipewire/pipewire.h>

#include <condition_variable>
#include <cstdint>
#include <memory>
#include <optional>
#include <string>
#include <vector>

#include "created_by_core.hpp"
#include "listener.hpp"

namespace stream::image::lin_impl
{

struct ScreenIdAndDescription
{
    uint32_t id;
    std::string name;
};
// ---------------- callbacks ---------------- //
void
registry_event_global(void* data,
                      uint32_t id,
                      uint32_t permissions,
                      const char* type,
                      uint32_t version,
                      const spa_dict* props);
// ---------------- callbacks ---------------- //

class Registry
{
public:
    Registry(const Registry&)            = delete;
    Registry(Registry&&)                 = delete;
    Registry& operator=(const Registry&) = delete;
    Registry& operator=(Registry&&)      = delete;

    Registry(pw_core*& core, pw_thread_loop* loop);
    ~Registry();

    const std::vector<ScreenIdAndDescription>& getIdsAndDescriptions();

private:
    friend void registry_event_global(void* data,
                                      uint32_t id,
                                      uint32_t permissions,
                                      const char* type,
                                      uint32_t version,
                                      const spa_dict* props);

    CreatedByCore m_syncer;

    pw_registry* m_registry;
    pw_registry_events m_register_events{};

    std::optional<Listener<pw_registry_events>> m_registry_listener;

    std::vector<ScreenIdAndDescription> m_screens_info;
};
} // namespace stream::image::lin_impl
