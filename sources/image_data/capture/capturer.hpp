#pragma once

#include <memory>

#include "../buf/proxy_view_in_video_buffer.hpp"

#include "ioutput.hpp"
#include <vector>

namespace stream::image
{
template<typename T>
concept OutConcept = requires(T& out){
     ScreenInfo(out.getInfo());
};

template<OutConcept Out> 
class ICapturer
{
public:

    ICapturer(std::vector<Out>&& outputs) {
        m_outputs = std::move(outputs);
    }

    ProxyViewInVideoBuffer capture() const{

    }
    bool pickScreen(uint32_t s_index){
        if(s_index >= m_outputs.size()) return false;
        m_cur_output = m_outputs[s_index];
    }
 
private:

    Out& m_cur_output;

    std::vector<Out> m_outputs;
};
} // namespace stream::image
