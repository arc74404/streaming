#include "win_texture_description.hpp"

#include <dxgiformat.h>

namespace stream::image::win_impl
{
TextureDescription::TextureDescription(
    const winrt::com_ptr<ID3D11Texture2D>& staging)
{
    staging->GetDesc(&m_desc);
}

uint32_t
TextureDescription::getWidth() const noexcept
{
    return m_desc.Width;
}

uint32_t
TextureDescription::getHeight() const noexcept
{
    return m_desc.Height;
}

uint32_t
TextureDescription::getChannelsCount() const noexcept
{
    return 4;
}

} // namespace stream::image::win_impl
