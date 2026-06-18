#pragma once

#include <string_view>

#include "../image_data/data_getter.hpp"

namespace stream::utils
{
bool
writeInFile(const image::Data& data, std::string_view filename);
}
