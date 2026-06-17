#include "win_output.hpp"

#include <winrt/Windows.Foundation.h>

#include <Windows.Graphics.Capture.Interop.h>
#include <Windows.Graphics.DirectX.Direct3D11.Interop.h>
#include <iostream>
#include <iterator>
#include <stdexcept>
#include <utility>

#pragma comment(lib, "windowsapp.lib")
#pragma comment(lib, "runtimeobject.lib")

namespace stream::image::win_impl
{

void
WinOutput::OnFrameArrived(const Capture::Direct3D11CaptureFramePool& sender,
                          const winrt::Windows::Foundation::IInspectable& args)
{
    std::lock_guard<std::mutex> lock(m_sync->mutex);
    m_sync->ready = true;

    auto frame = sender.TryGetNextFrame();

    if (frame)
    {
        IDirect3DSurface surface = frame.Surface();

        auto access = surface.as<Windows::Graphics::DirectX::Direct3D11::
                                     IDirect3DDxgiInterfaceAccess>();

        winrt::check_hresult(access->GetInterface(
            winrt::guid_of<ID3D11Texture2D>(), m_texture.put_void()));

        m_sync->success = true;
    }

    m_sync->cv.notify_one();
}

WinOutput::WinOutput(HWND hwnd, const IDirect3DDevice& device)
    : m_device(device)
{
    auto factory = winrt::get_activation_factory<
        winrt::Windows::Graphics::Capture::GraphicsCaptureItem,
        IGraphicsCaptureItemInterop>();

    winrt::check_hresult(factory->CreateForWindow(
        hwnd,
        winrt::guid_of<
            winrt::Windows::Graphics::Capture::IGraphicsCaptureItem>(),
        winrt::put_abi(m_capture_item)));

    m_frame_pool =
        winrt::Windows::Graphics::Capture::Direct3D11CaptureFramePool::
            CreateFreeThreaded(m_device,
                               winrt::Windows::Graphics::DirectX::
                                   DirectXPixelFormat::R8G8B8A8UIntNormalized,
                               2, m_capture_item.Size());

    m_session = m_frame_pool.CreateCaptureSession(m_capture_item);
}

void
WinOutput::createStagingBuffer()
{
    D3D11_TEXTURE2D_DESC desc;
    m_texture->GetDesc(&desc);

    D3D11_TEXTURE2D_DESC staging_desc = desc;
    staging_desc.Usage                = D3D11_USAGE_STAGING;
    staging_desc.BindFlags            = 0;
    staging_desc.CPUAccessFlags       = D3D11_CPU_ACCESS_READ;
    staging_desc.MiscFlags            = 0;

    winrt::com_ptr<ID3D11Device> d3d_device;

    auto access = m_device.as<::Windows::Graphics::DirectX::Direct3D11::
                                  IDirect3DDxgiInterfaceAccess>();

    winrt::check_hresult(access->GetInterface(winrt::guid_of<ID3D11Device>(),
                                              d3d_device.put_void()));

    winrt::check_hresult(d3d_device->CreateTexture2D(&staging_desc, nullptr,
                                                     m_staging_texture.put()));
}

void
WinOutput::start()
{
    if (false == in_process)
    {
        m_frame_arrived_token = m_frame_pool.FrameArrived(
            winrt::auto_revoke, [this](auto&& sender, auto&& args)
            { this->OnFrameArrived(sender, args); });

        m_sync = std::make_unique<SynchronizationData>();
        m_session.StartCapture();
        in_process = true;
    }
}

std::wstring
WinOutput::getNameImpl() const
{
    std::wstring name;
    name = m_capture_item.DisplayName();
    return name;
}

std::optional<WinOutput::ProxyBuffer>
WinOutput::captureImpl()
{
    start();

    std::unique_lock<std::mutex> lock(m_sync->mutex);
    m_sync->cv.wait(lock, [this] { return m_sync->ready; });

    if (m_sync->success)
    {
        if (!m_staging_texture) createStagingBuffer();

        return ProxyBuffer(m_texture, m_staging_texture, m_controler);
    }

    return std::nullopt;
}

} // namespace stream::image::win_impl
