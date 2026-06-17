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
Wire::IsValid() const noexcept
{
    return is_valid;
}

bool
Wire::loadBuffer() noexcept
{
    std::lock_guard lock(load_share_mutex);

    pw_buffer* buffer = nullptr;

    buffer = pw_stream_dequeue_buffer(m_stream);

    if (!buffer)
    {
        is_valid = false;
        goto ret_buffer;
    }
    else
    {
        spa_buffer* spa_buf = buffer->buffer;

        if (!spa_buf || !spa_buf->datas || !spa_buf->datas->data)
        {
            m_pixels.resize(0);
        }
        else
        {
            if(false == listen_flag) {
                goto ret_buffer;
            }
            CopyDataInPixels(spa_buf);
            listen_flag = false;
        }
    }

ret_buffer:

    pw_stream_queue_buffer(m_stream, buffer);  
    
    return is_valid;
}

void Wire::CopyDataInPixels(spa_buffer* buf){
    m_pixels.resize(buf->datas->chunk->size);
    memcpy(m_pixels.data(), buf->datas->data, m_pixels.size());

    bool tmp_w_was = false;

    uint32_t tmp_w;

    for(size_t i = 0; i < m_pixels.size() / 4; ++i) {
        uint32_t offset = i * 4;

        if(false == tmp_w_was && m_pixels[offset + 3] == 0) {
            tmp_w = i;
            tmp_w_was = true;
        }
        if(i % m_width == 1 && m_pixels[offset + 3] == 0){
            m_height = i / m_width;
            break;
        }

        uint8_t b = m_pixels[offset];
        uint8_t g = m_pixels[offset + 1];
        uint8_t r = m_pixels[offset + 2];
            
        m_pixels[offset] = r;
        m_pixels[offset + 1] = g;
        m_pixels[offset + 2] = b;
        m_pixels[offset + 3] = 255;
    }
    m_width = tmp_w;

    m_stride = buf->datas->chunk->stride;
}

void Wire::resetWH(uint32_t w, uint32_t h) noexcept
{
    m_width  = w;
    m_height = h;
}

bool
Wire::share(image::Data& getter)
{
    std::lock_guard lock(load_share_mutex);
    if (!is_valid) return false;

    if (m_pixels.size() == 0) return false;

    getter.data           = m_pixels.data();
    getter.row_pitch      = m_stride;
    getter.height         = m_height;
    getter.width          = m_width;
    getter.channels_count = 4;

    return getter.data;
}

} // namespace stream::image::lin_impl
