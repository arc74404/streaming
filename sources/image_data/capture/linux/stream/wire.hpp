#pragma once

#include <pipewire/pipewire.h>

#include <condition_variable>
#include <optional>

#include "../../../buf/data_getter.hpp"
#include "../listener.hpp"

namespace stream::image::lin_impl
{
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
    void listenOn();
    bool isListening() const;

    bool share(image::Data& getter) const;

    // api for callbacks
    bool loadBuffer() noexcept;

    bool IsValid() const noexcept;

    void resetWH(uint32_t w, uint32_t h) noexcept
    {
        m_width  = w;
        m_height = h;
    }

private:
    std::atomic<bool> listen_flag = false;

private:
    uint32_t m_width;
    uint32_t m_height;

    mutable std::mutex buf_for_load_mutex;
    bool buf_for_load = 0;

    std::atomic<bool> is_valid = true;

    pw_buffer* m_bufs[2] = {nullptr, nullptr};

    std::optional<Listener<pw_stream_events>> m_listener;

    pw_stream* m_stream;
};
} // namespace stream::image::lin_impl
