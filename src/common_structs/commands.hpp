#pragma once

#include <cstddef>

#include "struct_types.hpp"

namespace stream::structs
{

enum class Command : char
{
    GET_ID
};

#pragma pack(push, 1)
struct CommandPacket final
{
    CommandPacket(Command c, size_t user_id)
        : base(Type::COMMAND, user_id), command(c)
    {
    }

    BaseStruct base;

    Command command;
};
#pragma pack(pop)

CommandPacket
createCommand(Command com)
{
    return CommandPacket(com, 0);
}

} // namespace stream::structs
