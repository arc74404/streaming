#pragma once

#include <wrl/client.h>

#include <d3d11.h>
#include <vector>
#include <windows.h>

#include "physical_device.hpp"

namespace stream::image::win_impl
{
using Microsoft::WRL::ComPtr;
class ControlDevice final
{
public:
    ControlDevice(uint8_t level_count_support);

    PhysicalDevice GetPhysicalDevice() const;

private:
    ComPtr<ID3D11Device> m_device;
    ComPtr<ID3D11DeviceContext> m_immediate_context;
};
} // namespace stream::image::win_impl
