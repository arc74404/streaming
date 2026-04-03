#pragma once

#include <pipewire/pipewire.h>

#include <cstdint>
#include <memory>
#include <string>
#include <vector>

namespace stream::image::lin_impl
{

struct ScreenIdAndDescription
{
    uint32_t id;
    std::string name;
};

class Registry
{
public:
    Registry(pw_core* core);
    ~Registry();

    void pushInfo(ScreenIdAndDescription&& data);

private:
    pw_registry* m_registry;

    pw_registry_events m_events;

    spa_hook m_registry_listener;

    std::vector<ScreenIdAndDescription> m_screens_info;
};
} // namespace stream::image::lin_impl
