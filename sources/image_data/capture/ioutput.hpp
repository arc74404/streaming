#pragma once

#include <string>

namespace stream::image
{
struct ScreenInfo final
{
    std::string name;
};

class IOutput
{
public:
    virtual ~IOutput() = default;
    ScreenInfo getInfo()
    {
        return ScreenInfo{.name = getNameImpl()};
    }

private:
    virtual std::string getNameImpl() const = 0;
};
} // namespace stream::image
