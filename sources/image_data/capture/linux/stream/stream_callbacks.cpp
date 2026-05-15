#include "stream_callbacks.hpp"

#include <spa/param/video/format-utils.h>

#include <iostream>
#include <mutex>

#include "wire.hpp"

namespace stream::image::lin_impl
{
void
pw_stream_callbacks::on_process(void* user_data)
{
    std::cout << "on process\n";

    Wire* wire = static_cast<Wire*>(user_data);

    if (false == wire->isListening())
    {
        return;
    }

    wire->loadBuffer();
}

void
pw_stream_callbacks::on_param_changed(void* user_data,
                                      uint32_t id,
                                      const struct spa_pod* param)
{
    auto* self = static_cast<Wire*>(user_data);

    if (param == nullptr || id != SPA_PARAM_Format) return;

    spa_video_info_raw info{};
    if (spa_format_video_raw_parse(param, &info) < 0) return;

    self->resetWH(info.size.width, info.size.height);
}

void
pw_stream_callbacks::on_state_changed(void* user_data,
                                      enum pw_stream_state old,
                                      enum pw_stream_state new_state,
                                      const char* error_message)
{
    auto* self = static_cast<Wire*>(user_data);

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
