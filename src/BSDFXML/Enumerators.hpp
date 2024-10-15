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

    WindowElementType windowElementTypeFromString(std::string_view type);
    std::string windowElementTypeToString(WindowElementType type);

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
        LouveredShutter,
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

    enum class WavelengthDataDirection
    {
        Unknown,
        TransmissionFront,
        TransmissionBack,
        ReflectionFront,
        ReflectionBack,
        AbsorbedFront,
        AbsorbedBack
    };

    WavelengthDataDirection WavelengthDataDirectionFromString(std::string_view type);
    std::string WavelengthDataDirectionToString(WavelengthDataDirection type);

    enum class ScatteringDataType
    {
        Unknown,
        BTDF,
        FractionalDiffuse,
        FractionalSpecular
    };

    ScatteringDataType ScatteringDataTypeFromString(std::string_view type);
    std::string ScatteringDataTypeToString(ScatteringDataType type);

    enum class FileType
    {
        Unknown,
        BSDF,
        NonBSDF
    };

    FileType FileTypeFromString(std::string_view type);
    std::string FileTypeToString(FileType type);

    enum class WavelengthUnit
    {
        Unknown,
        Integral,
        Nanometer
    };

    WavelengthUnit WavelengthUnitFromString(std::string_view type);
    std::string WavelengthUnitToString(WavelengthUnit type);

    enum class LengthUnit
    {
        Unknown,
        Meter,
        Millimeter
    };

    LengthUnit LengthUnitFromString(std::string_view type);
    std::string LengthUnitToString(LengthUnit type);

    enum class ConductivityUnit
    {
        Unknown,
        WPerMeterKelvin
    };

    ConductivityUnit ConductivityUnitFromString(std::string_view type);
    std::string ConductivityUnitToString(ConductivityUnit type);
}   // namespace BSDFXML