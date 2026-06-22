#pragma once

namespace stream::structs
{
enum class Type : char
{
    STRING             = 0,
    SCREEN_META_PACKET = 1
};

#pragma pack(push, 1)
struct BaseStruct
{
    BaseStruct(Type t) : m_type(t) {};
    Type m_type;
};
#pragma pack(pop)
} // namespace stream::structs
