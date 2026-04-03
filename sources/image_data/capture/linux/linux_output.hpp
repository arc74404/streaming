#pragma once

#include <pipewire/pipewire.h>
#include <spa/param/video/raw.h>

#include <cstdint>

#include "../ioutput.hpp"

namespace stream::image::lin_impl
{

class Registry;

class LinuxOutput final : public IOutput<LinuxOutput>
{
public:
    std::wstring getNameImpl() const override;

    ProxyViewInVideoBuffer captureImpl() override;

    void setId(uint32_t id);

    void setFormatId(uint32_t fid);

    void init(const Registry& registry);

private:
    spa_video_info_raw m_video_info;

    uint32_t m_format_id;

    uint32_t m_id;
};
} // namespace stream::image::lin_impl
