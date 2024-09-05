#pragma once

#include <BSDFXML/Data.hpp>

namespace Helper
{
    void compareThickness(const BSDFXML::Thickness & expected, const BSDFXML::Thickness & actual);
    void compareThickness(const std::optional<BSDFXML::Thickness> & expected,
                          const std::optional<BSDFXML::Thickness> & actual);

    void compareMaterial(const BSDFXML::Material & expected, const BSDFXML::Material & actual);

    void compareThetaBounds(const BSDFXML::ThetaBounds & expected, const BSDFXML::ThetaBounds & actual);
    void compareAngleBasisBlock(const BSDFXML::AngleBasisBlock & expected, const BSDFXML::AngleBasisBlock & actual);
    void compareAngleBasis(const BSDFXML::AngleBasis & expected, const BSDFXML::AngleBasis & actual);
    void compareDataDefinition(const BSDFXML::DataDefinition & expected, const BSDFXML::DataDefinition & actual);
}   // namespace Helper