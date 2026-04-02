#pragma once

#include "physical_device.hpp"

#include <stdexcept>

namespace stream::image::win_impl
{
PhysicalDevice::PhysicalDevice(const ComPtr<ID3D11Device>& make_from)
{
    HRESULT hr = make_from.As(&m_device);

    if (FAILED(hr))
    {
        throw std::runtime_error("Failed to get IDXGIDevice from D3D11 Device");
    }
    hr = m_device->GetParent(IID_PPV_ARGS(&m_adapter));

    if (FAILED(hr))
    {
        throw std::runtime_error(
            "Failed to get IDXGIAdapter from IDXGI Device");
    }
}
std::vector<WinOutput>
PhysicalDevice::GetOutputs() const
{
    std::vector<WinOutput> result;

    uint32_t index = 0;

    ComPtr<IDXGIOutput> cur_out;

    while (m_adapter->EnumOutputs(index++, &cur_out) != DXGI_ERROR_NOT_FOUND)
    {
        result.emplace_back(std::move(cur_out));
    }

    return result;
}

} // namespace stream::image::win_impl
