#include "win_output.hpp"

#include <iterator>
#include <stdexcept>
#include <utility>
namespace stream::image::win_impl
{
WinOutput::WinOutput(ComPtr<IDXGIOutput>&& output)
{
    HRESULT hr = output.As(&m_output);
    if (FAILED(hr))
    {
        throw std::runtime_error("Failed create output(monitor)");
    }
}

std::wstring
WinOutput::getNameImpl() const
{
    DXGI_OUTPUT_DESC desc;
    m_output->GetDesc(&desc);

    std::wstring name;
    name.reserve(32);
    std::copy(desc.DeviceName, desc.DeviceName + 32,
              std::back_insert_iterator(name));
    return name;
}

ProxyViewInVideoBuffer
WinOutput::captureImpl()
{
    return ProxyViewInVideoBuffer(nullptr, 0);
}

} // namespace stream::image::win_impl
