#pragma once

#include <wrl/client.h>

#include <d3d11.h>
#include <vector>

#include "win_output.hpp"

namespace stream::image::win_impl
{
using Microsoft::WRL::ComPtr;
class PhysicalDevice final
{
public:
    PhysicalDevice(const ComPtr<ID3D11Device>& make_from);

    std::vector<WinOutput>& GetOutputs() const;

private:

    std::vector<WinOutput> m_outputs;

    ComPtr<IDXGIDevice> m_device;

    ComPtr<IDXGIAdapter> m_adapter;
};
} // namespace stream::image::win_impl
