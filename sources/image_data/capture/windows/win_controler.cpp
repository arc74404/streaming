#include "win_controler.hpp"

#include <winrt/Windows.Foundation.h>

#include <Windows.Graphics.Capture.Interop.h>
#include <Windows.Graphics.DirectX.Direct3D11.Interop.h>
#include <iostream>

#include "win_texture_description.hpp"

namespace stream::image::win_impl
{
bool
WinControler::updateDataOnGui(
    const winrt::com_ptr<ID3D11Texture2D>& proxy) noexcept
{
    // if (!proxy) return false;

    // winrt::com_ptr<ID3D11Device> device;
    // proxy->GetDevice(device.put());

    // winrt::com_ptr<ID3D11DeviceContext> context;
    // device->GetImmediateContext(context.put());

    // D3D11_MAPPED_SUBRESOURCE mapped;
    // HRESULT hr = context->Map(proxy.get(), 0, D3D11_MAP_READ, 0, &mapped);

    // if (SUCCEEDED(hr))
    // {
    //     context->Unmap(proxy.get(), 0);
    //     return true;
    // }

    return true;
}

bool
WinControler::fillStaging(
    const winrt::com_ptr<ID3D11Texture2D>& proxy,
    const winrt::com_ptr<ID3D11Texture2D>& staging) noexcept
{
    if (!proxy || !staging) return false;

    if (!m_d3d_device)
    {
        staging->GetDevice(m_d3d_device.put());
    }

    if (!m_context)
    {
        m_d3d_device->GetImmediateContext(m_context.put());
    }

    m_context->CopyResource(staging.get(), proxy.get());

    return true;
}

bool
WinControler::shareData(const winrt::com_ptr<ID3D11Texture2D>& staging,
                        Data& getter) noexcept
{
    if (!m_context || !m_d3d_device)
    {
        return false;
    }

    D3D11_MAPPED_SUBRESOURCE mapped;
    HRESULT hr = m_context->Map(staging.get(), 0, D3D11_MAP_READ, 0, &mapped);

    if (FAILED(hr)) return false;

    TextureDescription desc(staging);

    getter.data           = mapped.pData;
    getter.row_pitch      = mapped.RowPitch;
    getter.width          = desc.getWidth();
    getter.height         = desc.getHeight();
    getter.channels_count = desc.getChannelsCount();

    m_context->Unmap(staging.get(), 0);

    return true;
}

} // namespace stream::image::win_impl
