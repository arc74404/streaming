#include "control_device.hpp"

#include <stdexcept>
#include <string>

namespace stream::image::win_impl
{
ControlDevice::ControlDevice(uint8_t level_count_support)
{
    if (level_count_support > 3 || level_count_support < 1)
    {
        throw std::logic_error("level_count_support is in diaposon [1, 3]:" +
                               std::to_string(level_count_support));
    }

    D3D_FEATURE_LEVEL feature_levels[] = {
        D3D_FEATURE_LEVEL_11_1, D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_1};

    HRESULT hr;
    D3D_FEATURE_LEVEL feature_level;

    UINT creation_flags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;
#ifdef _DEBUG
    creation_flags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

    for (int i = 0; i < 2; ++i)
    {
        hr = D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr,
                               creation_flags, feature_levels,
                               level_count_support, D3D11_SDK_VERSION,
                               &m_device, &feature_level, &m_immediate_context);

        if (SUCCEEDED(hr))
        {
            break;
        }
        creation_flags &= ~D3D11_CREATE_DEVICE_DEBUG;
    }

    if (FAILED(hr))
    {
        throw std::runtime_error("Failed to create D3D11 Device. HRESULT: " +
                                 std::to_string(hr));
    }
}

PhysicalDevice
ControlDevice::GetPhysicalDevice() const
{
    return PhysicalDevice(m_device);
}

} // namespace stream::image::win_impl
