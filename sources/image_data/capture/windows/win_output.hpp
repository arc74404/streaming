#pragma once

#include <wrl/client.h>

#include <d3d11.h>
#include <dxgi1_2.h>
#include <string>

#include "../ioutput.hpp"

namespace stream::image::win_impl
{
using Microsoft::WRL::ComPtr;
class WinOutput final : public IOutput<WinOutput>
{
public:
    WinOutput(ComPtr<IDXGIOutput>&& output);

    std::wstring getNameImpl() const override;

    ProxyViewInVideoBuffer captureImpl() override;

private:
    ComPtr<IDXGIOutput1> m_output;
};
} // namespace stream::image::win_impl
