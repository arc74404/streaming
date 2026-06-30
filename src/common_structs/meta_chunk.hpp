#pragma once
#include <cstddef>

#include "struct_types.hpp"

namespace stream::structs
{
#pragma pack(push, 1)
struct MetaChunk final
{
    MetaChunk(size_t user_id, size_t i = 0)
        : base(Type::META_CHUNK, user_id), index(i)
    {
    }
    BaseStruct base;
    size_t index;
};
#pragma pack(pop)
} // namespace stream::structs
