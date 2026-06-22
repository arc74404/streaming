#pragma once

#include <algorithm>
#include <cstddef>
#include <cstdint>

#include "struct_types.hpp"

namespace stream::structs
{
#pragma pack(push, 1)
template <size_t str_size>
struct StringPacket final : private BaseStruct
{
    StringPacket(const char (&str)[str_size]) : BaseStruct(Type::STRING)
    {
        std::copy(str, str + str_size, m_val);
    }

    char m_val[str_size];
};
#pragma pack(pop)

template <size_t N>
StringPacket<N>
createStrPack(const char (&str)[N])
{
    return StringPacket<N>(str);
};

} // namespace stream::structs
