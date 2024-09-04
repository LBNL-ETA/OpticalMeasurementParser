#pragma once

#include <string>

#include "Data.hpp"

namespace BSDFXML
{
    WindowElement loadWindowElementFromFile(std::string_view fileName);
    int saveToFile(const WindowElement & model, std::string_view fileName);
}