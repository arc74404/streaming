#pragma once

#include <pipewire/pipewire.h>
#include <spa/param/video/format-utils.h>

namespace stream::image::lin_impl
{

class ConnectionParams final
{
public:
    ConnectionParams& setDirection(spa_direction val) noexcept
    {
        m_direction = val;
        return *this;
    }
    spa_direction getDirection() const noexcept
    {
        return m_direction;
    }

    ConnectionParams& setFlags(pw_stream_flags val) noexcept
    {
        m_flags = val;
        return *this;
    }
    pw_stream_flags getFlags() const noexcept
    {
        return m_flags;
    }
    ConnectionParams& setMediaType(spa_media_type val) noexcept
    {
        m_media_type = val;
        return *this;
    }
    spa_media_type getMediaType() const noexcept
    {
        return m_media_type;
    }
    ConnectionParams& setMediaSubType(spa_media_subtype val) noexcept
    {
        m_media_subtype = val;
        return *this;
    }
    spa_media_subtype getMediaSubType() const noexcept
    {
        return m_media_subtype;
    }
    ConnectionParams& setVideoFormat(spa_video_format val) noexcept
    {
        m_video_format = val;
        return *this;
    }
    spa_video_format getVideoFormat() const noexcept
    {
        return m_video_format;
    }

private:
    spa_direction m_direction;
    pw_stream_flags m_flags;
    spa_media_type m_media_type;
    spa_media_subtype m_media_subtype;
    spa_video_format m_video_format;
};

using StandartConnectionParams = ConnectionParams;
} // namespace stream::image::lin_impl
