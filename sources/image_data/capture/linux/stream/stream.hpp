#pragma once

#include <pipewire/pipewire.h>

#include <memory>
#include <optional>

#include "../created_by_core.hpp"
#include "../listener.hpp"

#include "connection_params.hpp"
#include "stream_callbacks.hpp"

#include <condition_variable>

namespace stream::image::lin_impl
{
struct StreamDeleter final
{
    StreamDeleter(CreatedByCore syncer) : m_syncer(syncer)
    {
    }

    void operator()(pw_stream* s)
    {
        if (s)
        {
            m_syncer.call(pw_stream_disconnect, s);
            m_syncer.call(pw_stream_destroy, s);
        }
    }
    CreatedByCore m_syncer;
};

class Stream final
{
public:
    struct Wire final
    {
    private:
        friend void pw_stream_callbacks::on_process(void* user_data);

        enum class Status
        {
            BAD,
            READY
        };
        std::condition_variable cv;
        mutable std::mutex mutex;

        Status status;

        pw_stream* stream;

        std::vector<uint8_t> pixels;
    };
    explicit Stream(pw_core* core, pw_thread_loop* loop);

    std::unique_ptr<Listener<pw_stream_events>> addListener(
        pw_stream_events callbacks_setup);

    bool connect(uint32_t window_index, const StandartConnectionParams& params);

private:
    std::shared_ptr<Wire> m_wire;

    CreatedByCore m_syncer;
    std::unique_ptr<pw_stream, StreamDeleter> m_stream;
};
} // namespace stream::image::lin_impl
