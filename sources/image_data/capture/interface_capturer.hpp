#pragma once

#include <algorithm>
#include <iterator>
#include <memory>
#include <stdexcept>
#include <vector>

#include "../buf/proxy_view_in_video_buffer.hpp"

#include "ioutput.hpp"

namespace stream::image
{
template <typename T>
concept OutConcept = requires(T& out) {
    ScreenInfo(out.getInfo());
    ScreenInfo(out);
    ProxyViewInVideoBuffer(out.capture().value());
};

template <OutConcept Out>
class ICapturer
{
public:
    ICapturer(std::vector<Out>* outputs = nullptr) : m_outputs(outputs)
    {
        if (m_outputs == nullptr || m_outputs->empty())
        {
            return;
        }
    }

    bool connectWith(std::vector<Out>* outputs) noexcept
    {
        if (outputs == nullptr)
        {
            return false;
        }
        m_outputs = outputs;
        return true;
    }

    bool isValid() const noexcept
    {
        return m_outputs != nullptr;
    }

    virtual bool prepare() noexcept = 0;

    std::vector<ScreenInfo> screensInfo() const
    {
        std::vector<ScreenInfo> result;
        std::copy(m_outputs->begin(), m_outputs->end(),
                  std::back_insert_iterator(result));
        return result;
    }

    decltype(auto) capture(size_t index) const noexcept
    {
        return m_outputs->operator[](index).capture();
    }

    bool pickScreen(uint32_t s_index) noexcept
    {
        if (s_index >= m_outputs->size()) return false;
        return true;
    }

private:
    std::vector<Out>* m_outputs;
};
} // namespace stream::image
