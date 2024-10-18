#pragma once

#include <bsdfdata/Data.hpp>

#include "ProductData.hpp"


namespace OpticsParser
{
    ProductData convert(const BSDFData::WindowElement & data);
    //BSDFData::WindowElement convert(const ProductData & data);
}   // namespace OpticsParser::BSDFData
