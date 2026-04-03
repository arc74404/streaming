#pragma once

#include <pipewire/pipewire.h>

#include <vector>

#include "registry.hpp"

namespace stream::image::lin_impl
{
class Core
{
public:
    Core();

    std::unique_ptr<Registry> createRegistry();

private:
    pw_main_loop* m_main_loop;
    pw_context* m_context;
    pw_core* m_core;
};
} // namespace stream::image::lin_impl
