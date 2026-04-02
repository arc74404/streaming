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
    ProxyViewInVideoBuffer(out.capture());
};

template <OutConcept Out>
class ICapturer
{
public:
    ICapturer(std::vector<Out>& outputs) : m_outputs(outputs)
    {
        if (m_outputs.empty())
        {
            throw std::invalid_argument("outputs should not be empty");
        }
        m_cur_output = &m_outputs[0];
    }

    std::vector<ScreenInfo> screensInfo() const
    {
        std::vector<ScreenInfo> result;
        std::copy(m_outputs.begin(), m_outputs.end(),
                  std::back_insert_iterator(result));
        return result;
    }

    ProxyViewInVideoBuffer capture() const
    {
        return m_cur_output->capture();
    }

    bool pickScreen(uint32_t s_index) noexcept
    {
        if (s_index >= m_outputs.size()) return false;
        m_cur_output = &m_outputs[s_index];
        return true;
    }

private:
    Out* m_cur_output;

    std::vector<Out>& m_outputs;
};
} // namespace stream::image
