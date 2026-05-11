#pragma once

#include "physical_device.hpp"

#include <Windows.Graphics.DirectX.Direct3D11.Interop.h>
#include <iostream>
#include <stdexcept>

namespace stream::image::win_impl
{
PhysicalDevice::PhysicalDevice(const ComPtr<ID3D11Device>& make_from)
{
    ComPtr<IDXGIDevice> device;

    HRESULT hr = make_from.As(&device);

    if (FAILED(hr))
    {
        throw std::runtime_error("Failed to get IDXGIDevice from D3D11 Device");
    }
    hr = CreateDirect3D11DeviceFromDXGIDevice(
        device.Get(),
        reinterpret_cast<::IInspectable**>(winrt::put_abi(m_device)));
    if (FAILED(hr))
    {
        throw std::runtime_error(
            "Failed to CreateDirect3D11DeviceFromDXGIDevice");
    }
}

BOOL
enumWindowsCallBack(HWND hwnd, LPARAM lParam)
{
    auto* self = reinterpret_cast<EnumWindowsCallBackData*>(lParam);

    if (IsWindowVisible(hwnd) && GetWindowTextLengthW(hwnd) > 0)
    {
        try
        {
            WinOutput new_out(hwnd, self->device);
            self->outputs.emplace_back(std::move(new_out));
        }
        catch (...)
        {
            return FALSE;
        }
        // catch (winrt::hresult_error& err)
        // { // std::wcout << err.message().c_str() << '\n';
        //     return FALSE;
        // }
    }
    return TRUE;
}

std::vector<WinOutput>
PhysicalDevice::getOutputs()
{
    std::vector<WinOutput> outputs;

    EnumWindowsCallBackData data = {.device = m_device, .outputs = outputs};

    EnumWindows(enumWindowsCallBack, reinterpret_cast<LPARAM>(&data));

    return outputs;
}

} // namespace stream::image::win_impl
