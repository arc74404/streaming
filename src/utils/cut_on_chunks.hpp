#pragma once

#include <boost/asio.hpp>

namespace stream
{

struct MetaChunk final
{
    size_t index;
};

inline uint32_t
calcRestInLastChunk(uint32_t size, uint32_t byte_size_per_chunk)
{
    return size % byte_size_per_chunk;
};

struct CutOnChunksResult final
{
    std::vector<std::array<boost::asio::const_buffer, 2>> buffers;
    std::vector<MetaChunk> storage_for_meta;
};

inline CutOnChunksResult
cutOnChunks(size_t size, const char* data, uint32_t byte_size_per_chunk)
{
    size_t count_chunks =
        (size + byte_size_per_chunk - 1) / byte_size_per_chunk;

    CutOnChunksResult result;
    result.buffers.reserve(count_chunks);
    result.storage_for_meta.resize(count_chunks);

    size_t cur_shift_ind = 0;
    while (cur_shift_ind < count_chunks)
    {
        MetaChunk meta{.index = cur_shift_ind};
        result.storage_for_meta[cur_shift_ind] = (std::move(meta));

        std::array<boost::asio::const_buffer, 2> packet_buffers = {
            boost::asio::buffer(result.storage_for_meta.data() + cur_shift_ind,
                                sizeof(MetaChunk)),
            boost::asio::buffer(data + cur_shift_ind * byte_size_per_chunk,
                                byte_size_per_chunk)};

        result.buffers.push_back(std::move(packet_buffers));

        ++cur_shift_ind;
    }
    std::cout << result.buffers.data() << '\n';
    std::cout << result.storage_for_meta.data() << '\n';
    return result;
}
} // namespace stream
