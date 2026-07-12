#pragma once
#include <algorithm>
#include <cstddef>
#include <cstring>
#include <stdexcept>
#include <turbojpeg.h>
#include <vector>

namespace stream
{
class JpegFarmingBufferOneCopy final
{
public:
    JpegFarmingBufferOneCopy& setQualitySize(int q)
    {
        m_quality = std::clamp(q, 1, 100);
        return *this;
    }
    JpegFarmingBufferOneCopy& setBlockSize(size_t bs)
    {
        m_block_byte_size = bs;
        resizeBuf();
        return *this;
    }
    JpegFarmingBufferOneCopy& setBlockCount(size_t bc)
    {
        m_block_need_count = bc;
        resizeBuf();
        return *this;
    }
    JpegFarmingBufferOneCopy& setMetaByteSize(size_t ms)
    {
        resizeBuf();
        return *this;
    }

    template <typename T>
    void setMeta(const T* data)
    {
        add(data, sizeof(T), 0);
    }
    void add(const void* data, size_t pos)
    {
        if (m_chunks_admin[pos])
        {
            return;
        }
        m_chunks_admin[pos] = true;

        size_t offset = pos * m_block_byte_size;

        if (offset > m_buffer.size())
        {
            throw std::invalid_argument("m_buffer.size() < offset in void "
                                        "FarmingBuffer::add");
        }
        std::memcpy(m_buffer.data() + offset, data, m_block_byte_size);
    }

    const uint8_t* data() const
    {
        return m_buffer.data();
    }

    bool frameReady() const
    {
        return std::find(m_chunks_admin.begin(), m_chunks_admin.end(), false) ==
               m_chunks_admin.end();
    }

private:
    void resizeBuf()
    {
        m_chunks_admin.resize(m_block_need_count);
        m_buffer.resize(m_block_byte_size * m_block_need_count);
    }
    std::vector<bool> m_chunks_admin; // true -> get, false -> no
    std::vector<uint8_t> m_buffer;

    int m_quality = 85;

    size_t m_block_byte_size  = 1;
    size_t m_block_need_count = 0;
};
} // namespace stream
