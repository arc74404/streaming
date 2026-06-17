#pragma once

#include <pipewire/pipewire.h>

#include <condition_variable>
#include <optional>

#include "../../../data_getter.hpp"
#include "../listener.hpp"

namespace stream::image::lin_impl
{
class Wire final
{
public:
    Wire(pw_stream* stream) : m_stream(stream)
    {
    }

    template <typename... ListenerArgs>
    void initListener(ListenerArgs&&... l_args)
    {
        m_listener.emplace(std::forward<ListenerArgs>(l_args)...);
    }
    void listenOn();

    bool share(image::Data& getter);

    // api for callbacks

    bool loadBuffer() noexcept;

    bool IsValid() const noexcept;

    void resetWH(uint32_t w, uint32_t h) noexcept;

private:
    std::atomic<bool> listen_flag = false;

private:

    void CopyDataInPixels(spa_buffer* buf);

    uint32_t m_width;
    uint32_t m_height;

    uint32_t m_stride;
    std::vector<uint8_t> m_pixels;

    mutable std::mutex load_share_mutex;

    std::atomic<bool> is_valid = true;

    int front_buffer;

    std::optional<Listener<pw_stream_events>> m_listener;

    pw_stream* m_stream;
};
} // namespace stream::image::lin_impl
