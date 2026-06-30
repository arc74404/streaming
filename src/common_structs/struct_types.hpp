#pragma once

namespace stream::structs
{
enum class Type : char
{
    STRING             = 0,
    SCREEN_META_PACKET = 1,
    META_CHUNK         = 2,
    COMMAND
};

#pragma pack(push, 1)
struct BaseStruct
{
    BaseStruct(Type t, size_t user_id) : m_type(t), m_user_id(user_id) {};
    Type m_type;
    size_t m_user_id;
};
#pragma pack(pop)
} // namespace stream::structs
