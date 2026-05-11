#pragma once

#include <winrt/Windows.Graphics.Capture.h>
#include <winrt/Windows.Graphics.DirectX.Direct3D11.h>
#include <wrl/client.h>

#include <d3d11.h>
#include <vector>

#include "win_output.hpp"

namespace stream::image::win_impl
{

using winrt::Windows::Graphics::DirectX::Direct3D11::IDirect3DDevice;

struct EnumWindowsCallBackData
{
    IDirect3DDevice& device;
    std::vector<WinOutput>& outputs;
};

BOOL
enumWindowsCallBack(HWND hwnd, LPARAM lParam);

using Microsoft::WRL::ComPtr;
class PhysicalDevice final
{
public:
    PhysicalDevice(const ComPtr<ID3D11Device>& make_from);

    std::vector<WinOutput> getOutputs();

private:
    IDirect3DDevice m_device;
};
} // namespace stream::image::win_impl
