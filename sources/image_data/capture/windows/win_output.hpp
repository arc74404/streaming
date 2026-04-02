#pragma once

#include <wrl/client.h>

#include <d3d11.h>
#include <dxgi1_2.h>
#include <string>

#include "../ioutput.hpp"

namespace stream::image::win_impl
{
using Microsoft::WRL::ComPtr;
class WinOutputImpl final : public IOutput
{
public:
    WinOutputImpl(ComPtr<IDXGIOutput>&& output);

    std::string getNameImpl() const override;

private:
    ComPtr<IDXGIOutput1> m_output;
};
} // namespace stream::image::win_impl
