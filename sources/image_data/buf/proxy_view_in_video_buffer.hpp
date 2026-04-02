#pragma once

namespace stream::image
{
struct ProxyViewInVideoBuffer
{
public:
    ProxyViewInVideoBuffer(void* view, size_t byte_count)
        : m_view(view), m_byte_count(byte_count)
    {
    }
    void setView(void* view, size_t byte_count)
    {
        m_view       = view;
        m_byte_count = byte_count;
    }

private:
    void* m_view;
    size_t m_byte_count;
};
} // namespace stream::image
