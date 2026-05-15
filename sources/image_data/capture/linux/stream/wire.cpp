#include "wire.hpp"

#include <spa/buffer/meta.h>

#include <iostream>
namespace stream::image::lin_impl
{
void
Wire::listenOn()
{
    listen_flag = true;
}

bool
Wire::isListening() const
{
    return listen_flag;
}

bool
Wire::IsValid() const noexcept
{
    return is_valid;
}

bool
Wire::loadBuffer() noexcept
{
    if (m_bufs[buf_for_load])
    {
        pw_stream_queue_buffer(m_stream, m_bufs[buf_for_load]);
    }
    m_bufs[buf_for_load] = pw_stream_dequeue_buffer(m_stream);
    if (!m_bufs[buf_for_load])
    {
        is_valid = false;
    }

    return is_valid;
}

bool
Wire::share(image::Data& getter) const
{
    bool buf_for_user;
    {
        std::unique_lock lock(buf_for_load_mutex);
        buf_for_user = ~buf_for_load;
    }
    if (!is_valid) return false;

    if (!m_bufs[buf_for_user]) return false;

    spa_buffer* spa_buf = m_bufs[buf_for_user]->buffer;

    if (!spa_buf || !spa_buf->datas || !spa_buf->datas->data)
    {
        getter.data = nullptr;
        return false;
    }

    getter.data           = m_bufs[buf_for_user]->buffer->datas->data;
    getter.row_pitch      = spa_buf->datas[0].chunk->stride;
    getter.height         = m_height;
    getter.width          = m_width;
    getter.channels_count = 4;

    return getter.data;
}

} // namespace stream::image::lin_impl
