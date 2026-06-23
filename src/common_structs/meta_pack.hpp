#pragma once
#include <cstdint>

#include "struct_types.hpp"

namespace stream::structs
{
#pragma pack(push, 1)
struct ScreenMetaPacket final
{
    ScreenMetaPacket() : base(Type::SCREEN_META_PACKET)
    {
    }

    BaseStruct base;

    uint32_t height;
    uint32_t width;
    uint32_t packet_size;
};
#pragma pack(pop)
} // namespace stream::structs
