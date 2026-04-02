#pragma once

#include "../ioutput.hpp"

namespace stream::image::lin_impl
{
class LinuxOutput final : public IOutput<LinuxOutput>
{
public:
    std::wstring getNameImpl() const override;

private:
};
} // namespace stream::image::lin_impl
