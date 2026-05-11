#pragma once

#include <string_view>

#include "buf/data_getter.hpp"

namespace stream::image
{
class Screener final
{
public:
    bool write(const Data& data, std::string_view filename);

private:
};
} // namespace stream::image
