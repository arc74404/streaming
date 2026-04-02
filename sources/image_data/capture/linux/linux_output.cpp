#include "linux_output.hpp"

namespace stream::image::lin_impl
{
std::wstring
LinuxOutput::getNameImpl() const
{
    return L"none";
}
} // namespace stream::image::lin_impl
