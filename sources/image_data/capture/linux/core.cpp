#include "core.hpp"

#include <functional>
#include <iostream>
#include <stdexcept>

#include "sync_func.hpp"

namespace stream::image::lin_impl
{

Core::PwInit::PwInit()
{
    pw_init(nullptr, nullptr);
}
Core::PwInit::~PwInit()
{
    pw_deinit();
}

Core::Core()
{
    m_loop.reset(pw_thread_loop_new("core-loop", nullptr));

    if (!m_loop)
    {
        throw std::runtime_error("Failed create loop");
    }

    m_context.reset(
        pw_context_new(pw_thread_loop_get_loop(m_loop.get()), nullptr, 0));

    if (!m_context)
    {
        throw std::runtime_error("Failed create context");
    }

    if (pw_thread_loop_start(m_loop.get()) < 0)
    {
        throw std::runtime_error("Failed start loop");
    }

    lock();

    m_core = pw_context_connect(m_context.get(), nullptr, 0);
    if (!m_core)
    {
        unlock();
        throw std::runtime_error("Failed connect core");
    }

    unlock();
}

Core::~Core() noexcept
{
    lock();

    if (m_core)
    {
        sync();
        pw_core_disconnect(m_core);
    }
    pw_thread_loop_signal(m_loop.get(), false);
    unlock();

    pw_thread_loop_stop(m_loop.get());
}

std::unique_ptr<Registry>
Core::createRegistry()
{
    lock();
    auto reg = std::make_unique<Registry>(m_core, m_loop.get());
    sync();
    unlock();
    return reg;
}

Stream
Core::createStream()
{
    lock();
    Stream stream(m_core, m_loop.get());
    unlock();
    return stream;
}
void
Core::lock()
{
    lin_impl::lock(m_loop.get());
}
void
Core::unlock()
{
    lin_impl::unlock(m_loop.get());
}
void
Core::wait()
{
    lin_impl::wait(m_loop.get());
}
void
Core::signal()
{
    lin_impl::signal(m_loop.get());
}

pw_thread_loop*
Core::loop()
{
    return m_loop.get();
}
struct SyncData
{
    int target_seq;
    Core* core;
    bool ready = false;
};

static void
on_core_done(void* data, uint32_t id, int seq)
{
    SyncData* sync = reinterpret_cast<SyncData*>(data);

    if (id == PW_ID_CORE && seq == sync->target_seq)
    {
        sync->ready = true;
        sync->core->signal();
    }
}

bool
Core::sync()
{
    SyncData sync_data;
    sync_data.core       = this;
    sync_data.target_seq = -1;

    pw_core_events core_events = {.done = on_core_done};
    core_events.version        = PW_VERSION_CORE_EVENTS;

    Listener<pw_core_events> core_listener(
        [this](spa_hook* listener, pw_core_events* events, SyncData& data)
        {
            pw_core_add_listener(m_core, listener, events,
                                 reinterpret_cast<void*>(&data));
            return true;
        },
        core_events, sync_data);

    int seq = pw_core_sync(m_core, PW_ID_CORE, 0);

    if (seq < 0)
    {
        return false;
    }
    sync_data.target_seq = seq;

    while (false == sync_data.ready)
    {
        wait();
    }

    return true;
}
} // namespace stream::image::lin_impl
