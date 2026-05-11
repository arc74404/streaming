#pragma once

#include <winrt/Windows.Graphics.Capture.h>

#include <d3d11.h>
#include <dxgi1_2.h>

namespace stream::image::win_impl
{
class TextureDescription final
{
public:
    TextureDescription(const winrt::com_ptr<ID3D11Texture2D>& staging);

    uint32_t getWidth() const noexcept;
    uint32_t getHeight() const noexcept;

    uint32_t getChannelsCount() const noexcept;

private:
    D3D11_TEXTURE2D_DESC m_desc;
};
} // namespace stream::image::win_impl



