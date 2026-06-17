#pragma once

#include <winrt/Windows.Graphics.Capture.h>
#include <winrt/Windows.Graphics.DirectX.Direct3D11.h>
#include <wrl/client.h>

#include <condition_variable>
#include <d3d11.h>
#include <dxgi1_2.h>
#include <memory>
#include <mutex>
#include <optional>
#include <string>

#include "../ioutput.hpp"

#include "win_controler.hpp"

namespace stream::image::win_impl
{

using Microsoft::WRL::ComPtr;
using winrt::Windows::Graphics::DirectX::Direct3D11::IDirect3DDevice;
using winrt::Windows::Graphics::DirectX::Direct3D11::IDirect3DSurface;
using namespace winrt::Windows::Graphics;

class WinOutput final : public IOutput<WinOutput>
{
public:
    using ProxyBuffer = ProxyViewInVideoBuffer<winrt::com_ptr<ID3D11Texture2D>,
                                               winrt::com_ptr<ID3D11Texture2D>,
                                               WinControler>;

    WinOutput(HWND hwnd, const IDirect3DDevice& device);

    std::wstring getNameImpl() const;

    std::optional<ProxyBuffer> captureImpl();

    

private:
    void createStagingBuffer();
    
    // ----------- start ----------- //
    bool in_process = false;

    struct SynchronizationData
    {
        bool ready   = false;
        bool success = false;
        std::mutex mutex;
        std::condition_variable cv;
    };

    std::unique_ptr<SynchronizationData> m_sync{nullptr};

    void start();
    // ----------------------------- //

    const IDirect3DDevice& m_device;

    Capture::GraphicsCaptureItem m_capture_item{nullptr};
    Capture::Direct3D11CaptureFramePool m_frame_pool{nullptr};
    Capture::GraphicsCaptureSession m_session{nullptr};

    void OnFrameArrived(const Capture::Direct3D11CaptureFramePool& sender,
                        const winrt::Windows::Foundation::IInspectable& args);

    Capture::Direct3D11CaptureFramePool::FrameArrived_revoker
        m_frame_arrived_token;

    WinControler m_controler;
    winrt::com_ptr<ID3D11Texture2D> m_texture{nullptr};
    winrt::com_ptr<ID3D11Texture2D> m_staging_texture{nullptr};
};
} // namespace stream::image::win_impl
