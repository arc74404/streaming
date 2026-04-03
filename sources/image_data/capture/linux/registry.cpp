#include "registry.hpp"

#include <stdexcept>

namespace stream::image::lin_impl
{
Registry::Registry()
{
    pw_init(nullptr, nullptr);

    m_main_loop = pw_main_loop_new(NULL);
    if (!m_main_loop)
    {
        throw std::runtime_error("Failed create main loop in registry");
    }
    m_context = pw_context_new(pw_main_loop_get_loop(m_main_loop), NULL, 0);

    m_core = pw_context_connect(context, NULL, 0);
    if (!m_core)
    {
        throw std::runtime_error("Failed create core in registry");
    }
}
std::vector<LinuxOutput>&
Registry::getOutputs()
{
    return m_outputs;
}
} // namespace stream::image::lin_impl
