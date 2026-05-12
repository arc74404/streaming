#include "stream.hpp"

#include <spa/param/video/raw.h>

#include <iostream>
#include <stdexcept>

namespace stream::image::lin_impl
{
Stream::Stream(pw_core* core, pw_thread_loop* loop)
    : m_stream(nullptr, StreamDeleter(CreatedByCore(loop))), m_syncer(loop)
{
    auto* data = pw_stream_new(core, nullptr, nullptr);
    if (nullptr == data)
    {
        throw std::runtime_error("Failed create stream");
    }
    m_stream.reset(data);
}

bool
Stream::connect(uint32_t window_index,
                const StandartConnectionParams& con_params)
{
    constexpr size_t count_params = 1;

    const spa_pod* params[count_params];
    uint8_t buffer[1024];
    spa_pod_builder b = SPA_POD_BUILDER_INIT(buffer, sizeof(buffer));

    params[0] = (spa_pod*)spa_pod_builder_add_object(
        &b, SPA_TYPE_OBJECT_Format, SPA_PARAM_EnumFormat, SPA_FORMAT_mediaType,
        SPA_POD_Id(con_params.getMediaType()), SPA_FORMAT_mediaSubtype,
        SPA_POD_Id(con_params.getMediaSubType()), SPA_FORMAT_VIDEO_format,
        SPA_POD_Id(con_params.getVideoFormat()));

    int res = m_syncer.call(pw_stream_connect, m_stream.get(),
                            con_params.getDirection(), window_index,
                            con_params.getFlags(), params, count_params);
    return res == 0;
}

std::unique_ptr<Stream::Wire>
Stream::CreateWire(pw_stream_events callbacks_setup) noexcept
{
    try
    {
        std::unique_ptr<Stream::Wire> result =
            std::make_unique<Wire>(m_stream.get());

        auto creater =
            [this, &result](spa_hook* listener, const pw_stream_events* events)
        {
            m_syncer.call(pw_stream_add_listener, m_stream.get(), listener,
                          events, result.get());
            if (!listener)
            {
                return false;
            }
            return true;
        };
        result->initListener(creater, callbacks_setup);
        return std::move(result);
    }
    catch (...)
    {
        return nullptr;
    }
}
} // namespace stream::image::lin_impl
