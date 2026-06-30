#pragma once

#include <algorithm>
#include <cstddef>
#include <cstdint>

#include "struct_types.hpp"

namespace stream::structs
{
#pragma pack(push, 1)
template <size_t str_size>
struct StringPacket final
{
    StringPacket(const char (&str)[str_size], size_t user_id)
        : base(Type::STRING, user_id)
    {
        std::copy(str, str + str_size, m_val);
    }

    BaseStruct base;

    char m_val[str_size];
};
#pragma pack(pop)

template <size_t N>
StringPacket<N>
createStrPack(const char (&str)[N], size_t user_id)
{
    return StringPacket<N>(str, user_id);
};

} // namespace stream::structs
