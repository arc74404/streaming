#pragma once

#include <winrt/Windows.Graphics.Capture.h>
#include <winrt/Windows.Graphics.DirectX.Direct3D11.h>
#include <wrl/client.h>

#include <d3d11.h>
#include <dxgi1_2.h>
#include <string>

#include "../../buf/data_getter.hpp"

namespace stream::image::win_impl
{
class WinControler final
{
public:
    bool updateDataOnGui(const winrt::com_ptr<ID3D11Texture2D>& proxy) noexcept;

    bool fillStaging(const winrt::com_ptr<ID3D11Texture2D>& proxy,
                     const winrt::com_ptr<ID3D11Texture2D>& staging) noexcept;

    bool shareData(const winrt::com_ptr<ID3D11Texture2D>& staging,
                   Data& getter) noexcept;

private:
    winrt::com_ptr<ID3D11DeviceContext> m_context{nullptr};
    winrt::com_ptr<ID3D11Device> m_d3d_device{nullptr};
};
} // namespace stream::image::win_impl
