#pragma once

#include <pipewire/pipewire.h>

#include <vector>

namespace stream::image::lin_impl
{
class Registry
{
public:
    Registry();

    std::vector<lin_impl::LinuxOutput>& getOutputs();

private:
    pw_main_loop* m_main_loop;
    pw_context* m_context;
    pw_core* m_core;

    std::vector<lin_impl::LinuxOutput> m_outputs;
};
} // namespace stream::image::lin_impl
