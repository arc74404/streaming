#include "stream_callbacks.hpp"

#include <iostream>
#include <mutex>

#include "stream.hpp"

namespace stream::image::lin_impl
{
void
pw_stream_callbacks::on_process(void* user_data)
{
    std::cout << "on process\n";
    Stream::Wire* wire = static_cast<Stream::Wire*>(user_data);

    pw_buffer* buf = pw_stream_dequeue_buffer(wire->m_stream);

    if (buf == nullptr)
    {
        {
            std::lock_guard lock(wire->mutex);
            wire->status = Stream::Wire::Status::BAD;
        }
        wire->cv.notify_all();
        return;
    }
    spa_data* data = nullptr;

    if (nullptr == buf->buffer)
    {
        goto queue_buffer;
    }

    data = &buf->buffer->datas[0];

    if (data->data)
    {
        {
            std::lock_guard lock(wire->mutex);
            wire->pixels.resize(data->chunk->size);
            memcpy(wire->pixels.data(), data->data, data->chunk->size);
            wire->status = Stream::Wire::Status::READY;
        }
        wire->cv.notify_one();
    }

queue_buffer:
    pw_stream_queue_buffer(wire->m_stream, buf);
}

void
pw_stream_callbacks::on_param_changed(void* user_data,
                                      uint32_t id,
                                      const struct spa_pod* param)
{
    auto* self = static_cast<Stream::Wire*>(user_data);

    if (param == nullptr || id != SPA_PARAM_Format) return;
}

void
pw_stream_callbacks::on_state_changed(void* user_data,
                                      enum pw_stream_state old,
                                      enum pw_stream_state new_state,
                                      const char* error_message)
{
    auto* self = static_cast<Stream::Wire*>(user_data);

    switch (new_state)
    {
        case PW_STREAM_STATE_STREAMING:
            break;

        case PW_STREAM_STATE_ERROR:
            break;

        case PW_STREAM_STATE_UNCONNECTED:
            break;

        case PW_STREAM_STATE_CONNECTING:
            break;

        case PW_STREAM_STATE_PAUSED:
            break;
    }
}

pw_stream_events
pw_stream_callbacks::standartCallbacks()
{
    pw_stream_events result{};
    result.version       = PW_VERSION_STREAM_EVENTS;
    result.state_changed = on_state_changed;
    result.param_changed = on_param_changed;
    result.process       = on_process;
    return result;
}

} // namespace stream::image::lin_impl
