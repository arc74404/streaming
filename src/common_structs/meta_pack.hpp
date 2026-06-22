#pragma once
#include <cstdint>

#include "struct_types.hpp"

namespace stream::structs
{
#pragma pack(push, 1)
struct ScreenMetaPacket final : private BaseStruct
{
    ScreenMetaPacket() : BaseStruct(Type::SCREEN_META_PACKET)
    {
    }

    uint32_t height;
    uint32_t width;
    uint32_t packet_size;
};
#pragma pack(pop)
} // namespace stream::structs
