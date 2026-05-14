#include "linux_capturer.hpp"

#include <pipewire/pipewire.h>

#include <algorithm>
#include <iostream>
#include <ranges>

#include "dbus/dbus_requests.hpp"

namespace stream::image
{
LinuxCapturer::LinuxCapturer()
{
    connectWith(&m_outputs);
}

bool
LinuxCapturer::prepare() noexcept
{
    auto create_session_answer = dbus::CreateScreenPickSession(m_bus);

    if (false == create_session_answer.has_value())
    {
        return false;
    }

    auto get_session_handle =
        GetSessionHandle(m_bus, create_session_answer.value().m_request_handle);

    if (false == get_session_handle.has_value())
    {
        return false;
    }

    auto select_sources_answer =
        dbus::SelectSources(m_bus, get_session_handle.value());

    if (false == select_sources_answer.has_value())
    {
        return false;
    }

    if (false ==
        WaitSourcesHandle(
            m_bus, select_sources_answer.value().m_sources_handle_address))
    {
        return false;
    }

    auto start_res = dbus::Start(m_bus, get_session_handle.value());

    if (false == start_res.has_value())
    {
        return false;
    }

    auto wait_user_res =
        dbus::WaitUser(m_bus, start_res.value().m_start_result);

    if (false == wait_user_res.has_value())
    {
        return false;
    }

    m_picked_index = wait_user_res.value();

    if (false == start())
    {
        return false;
    }
    return true;
}

bool
LinuxCapturer::start() noexcept
{
    try
    {
        std::unique_ptr<lin_impl::Registry> registry = m_core.createRegistry();

        std::vector<stream::image::lin_impl::ScreenIdAndDescription> info =
           registry->getIdsAndDescriptions();

        if (false == setupOutputs())
        {
            return false;
        }
    }
    catch (...)
    {
        return false;
    }
    return true;
}

bool
LinuxCapturer::setupOutputs()
{
    lin_impl::Stream stream = m_core.createStream();

    bool success = stream.connect(
        m_picked_index,
        lin_impl::StandartConnectionParams()
            .setDirection(SPA_DIRECTION_INPUT)
            .setFlags(pw_stream_flags(PW_STREAM_FLAG_AUTOCONNECT |
                                      PW_STREAM_FLAG_MAP_BUFFERS |
                                      PW_STREAM_FLAG_ALLOC_BUFFERS))
            .setMediaType(spa_media_type::SPA_MEDIA_TYPE_video)
            .setMediaSubType(spa_media_subtype::SPA_MEDIA_SUBTYPE_raw)
            .setVideoFormat(spa_video_format::SPA_VIDEO_FORMAT_BGRx));

    if (false == success)
    {
        return false;
    }

    m_streams.emplace_back(std::move(stream));

    std::unique_ptr<lin_impl::Stream::Wire> wire = m_streams[0].CreateWire(
        lin_impl::pw_stream_callbacks::standartCallbacks());

    if (nullptr == wire)
    {
        return false;
    }

    m_outputs.emplace_back(std::move(wire));
    return true;
}

} // namespace stream::image
