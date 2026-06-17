#pragma once

#include <pipewire/pipewire.h>

#include <memory>
#include <utility>

#include "stream/stream.hpp"

#include "registry.hpp"

namespace stream::image::lin_impl
{
class Core
{
public:
    Core();
    ~Core() noexcept;

    std::unique_ptr<Registry> createRegistry();
    Stream createStream();

    void lock();
    void unlock();
    void wait();
    void signal();

    bool sync();

    pw_thread_loop* loop();

private:
    struct PwInit
    {
        PwInit();
        ~PwInit();
    } m_pw_init;

    std::unique_ptr<pw_thread_loop, decltype(&pw_thread_loop_destroy)> m_loop{
        nullptr, pw_thread_loop_destroy};
    std::unique_ptr<pw_context, decltype(&pw_context_destroy)> m_context{
        nullptr, pw_context_destroy};
    pw_core* m_core = nullptr;
};

} // namespace stream::image::lin_impl
