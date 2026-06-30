#pragma once

#include <string>

namespace stream
{
inline std::string
generateId()
{
    static size_t num = 5;
    std::string id    = std::to_string(num);
    ++num;
    return "id: " + id;
}
} // namespace stream
