#pragma once

#include <string>

namespace stream
{
inline std::pair<size_t, std::string>
generateId()
{
    static size_t num = 5;
    std::string id    = std::to_string(num);
    return {num++, "id: " + id};
}
} // namespace stream
