#pragma once

#include <cstdint>
#include <functional>

namespace stream
{
struct Chunk
{
    uint32_t index;
    uint32_t byte_data_count;
    void* data;
};

class ChunkCutter final
{
public:
    struct Info
    {
        uint32_t count_chunks;
        uint32_t rest_bytes_last_chunk;
    };
    ChunkCutter& setBlockSize(uint32_t byte_size);

private:
    std::function<void()> binder;
};
}; // namespace stream
