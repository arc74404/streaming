#pragma once

#include <pipewire/pipewire.h>

#include "../../buf/data_getter.hpp"

namespace stream::image::lin_impl
{
class LinControler final
{
public:
    bool updateDataOnGui();

    bool fillStaging(pw_buffer& buffer);

    bool shareData(pw_buffer& buffer, Data& data);

private:
};
} // namespace stream::image::lin_impl
