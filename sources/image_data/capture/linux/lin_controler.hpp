#pragma once

#include <pipewire/pipewire.h>

#include "../../buf/data_getter.hpp"
#include "stream/stream.hpp"

namespace stream::image::lin_impl
{
class LinControler final
{
public:
    LinControler(Stream::Wire& wire);

    bool updateDataOnGui();

    bool fillStaging();

    bool shareData(Data& data);

private:
    Stream::Wire& m_wire;
};
} // namespace stream::image::lin_impl
