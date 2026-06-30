#pragma once

#include <boost/asio.hpp>

#include "../common_structs/meta_chunk.hpp"

namespace stream
{
inline uint32_t
calcRestInLastChunk(uint32_t size, uint32_t byte_size_per_chunk)
{
    return size % byte_size_per_chunk;
};

struct CutOnChunksResult final
{
    std::vector<std::array<boost::asio::const_buffer, 2>> buffers;
    std::vector<structs::MetaChunk> storage_for_meta;
};

inline CutOnChunksResult
cutOnChunks(size_t size, const char* data, uint32_t byte_size_per_chunk)
{
    size_t count_chunks =
        (size + byte_size_per_chunk - 1) / byte_size_per_chunk;

    CutOnChunksResult result;
    result.buffers.reserve(count_chunks);
    result.storage_for_meta.reserve(count_chunks);

    size_t cur_shift_ind = 0;
    while (cur_shift_ind < count_chunks)
    {
        structs::MetaChunk meta(0, cur_shift_ind);

        result.storage_for_meta.emplace_back(std::move(meta));

        std::array<boost::asio::const_buffer, 2> packet_buffers = {
            boost::asio::buffer(result.storage_for_meta.data() + cur_shift_ind,
                                sizeof(structs::MetaChunk)),
            boost::asio::buffer(data + cur_shift_ind * byte_size_per_chunk,
                                byte_size_per_chunk)};

        result.buffers.push_back(std::move(packet_buffers));

        ++cur_shift_ind;
    }
    return result;
}
} // namespace stream
