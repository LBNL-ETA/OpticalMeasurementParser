#pragma once

#include <BSDFXML/Data.hpp>

namespace Helper
{
    void compareThickness(const BSDFXML::Thickness & expected, const BSDFXML::Thickness & actual);
    void compareThickness(const std::optional<BSDFXML::Thickness> & expected,
                          const std::optional<BSDFXML::Thickness> & actual);

    void compareWavelength(const BSDFXML::Wavelength & expected, const BSDFXML::Wavelength & actual);
    void compareWavelength(const std::optional<BSDFXML::Wavelength> & expected,
                          const std::optional<BSDFXML::Wavelength> & actual);

    void compareMaterial(const BSDFXML::Material & expected, const BSDFXML::Material & actual);

    void compareThetaBounds(const BSDFXML::ThetaBounds & expected, const BSDFXML::ThetaBounds & actual);
    void compareAngleBasisBlock(const BSDFXML::AngleBasisBlock & expected, const BSDFXML::AngleBasisBlock & actual);
    void compareAngleBasis(const BSDFXML::AngleBasis & expected, const BSDFXML::AngleBasis & actual);
    void compareDataDefinition(const BSDFXML::DataDefinition & expected, const BSDFXML::DataDefinition & actual);

    void compareScatteringData(const BSDFXML::ScatteringData & expected, const BSDFXML::ScatteringData & actual);
    void compareWavelengthDataBlock(const BSDFXML::WavelengthDataBlock & expected, const BSDFXML::WavelengthDataBlock & actual);
    void compareWavelengthData(const BSDFXML::WavelengthData & expected, const BSDFXML::WavelengthData & actual);
}   // namespace Helper