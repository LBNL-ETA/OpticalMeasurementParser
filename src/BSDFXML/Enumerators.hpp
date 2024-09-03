//! \file Enumerators.hpp
//! \brief Contains enumerators for the BSDF XML file format.
//! Mapping is taken from the BSDF XML schema.

#pragma once

#include <string>

namespace BSDFXML
{
    enum class WindowElementType
    {
        Unknown,
        System,
        Layer,
        Material
    };

    WindowElementType windowElementFromString(std::string_view type);
    std::string windowElementToString(WindowElementType type);

    enum class DeviceType
    {
        Unknown,
        VenetianBlindHorizontal,
        VenetianBlindVertical,
        WovenShade,
        DiffusingGlass,
        SpecularGlass,
        FrittedGlass,
        CellularShade,
        PleatedShade,
        SolidLayer,
        Other
    };

    DeviceType DeviceTypeFromString(std::string_view type);
    std::string DeviceTypeToString(DeviceType type);

    enum class IncidentDataStructure
    {
        Unknown,
        Columns
    };

    IncidentDataStructure IncidentDataStructureFromString(std::string_view type);
    std::string IncidentDataStructureToString(IncidentDataStructure type);
}   // namespace BSDFXML