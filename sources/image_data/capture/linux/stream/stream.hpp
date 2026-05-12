#pragma once

#include <pipewire/pipewire.h>

#include <condition_variable>
#include <iostream>
#include <memory>
#include <optional>

#include "../created_by_core.hpp"
#include "../listener.hpp"

#include "connection_params.hpp"
#include "stream_callbacks.hpp"

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
            auto delete_lambda = [this, &s]()
            {
                std::cout << "delete stream\n";
                pw_stream_disconnect(s);
                pw_stream_destroy(s);
            };
            m_syncer.call(delete_lambda);
        }
    }
    CreatedByCore m_syncer;
};

class Stream final
{
public:
    struct Wire final
    {
        Wire(pw_stream* stream) : m_stream(stream)
        {
        }

        template <typename... ListenerArgs>
        void initListener(ListenerArgs&&... l_args)
        {
            m_listener.emplace(std::forward<ListenerArgs>(l_args)...);
        }
        void listen() noexcept
        {
            std::lock_guard lock(mutex);
            listen_flag = true;
        }

    private:
        bool listen_flag = false;

    private:
        friend void pw_stream_callbacks::on_process(void* user_data);

        enum class Status
        {
            BAD,
            READY
        };
        std::optional<Listener<pw_stream_events>> m_listener;

        std::condition_variable cv;
        mutable std::mutex mutex;

        Status status;

        pw_stream* m_stream;

        std::vector<uint8_t> pixels;
    };
    explicit Stream(pw_core* core, pw_thread_loop* loop);

    std::unique_ptr<Wire> CreateWire(pw_stream_events callbacks_setup) noexcept;

    bool connect(uint32_t window_index, const StandartConnectionParams& params);

private:
    CreatedByCore m_syncer;

    std::unique_ptr<pw_stream, StreamDeleter> m_stream;
};
} // namespace stream::image::lin_impl
