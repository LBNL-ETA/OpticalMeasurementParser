#include <array>

#include <fileParse/StringConversion.hxx>

#include "Enumerators.hpp"

namespace BSDFXML
{
    const std::array<std::string, 4> WindowElementTypeStrings = {
      "Unknown", "System", "Layer", "Material"};

    WindowElementType windowElementTypeFromString(std::string_view type)
    {
        return FileParse::enumFromString(
          type, WindowElementTypeStrings, WindowElementType::Unknown);
    }

    std::string windowElementTypeToString(WindowElementType type)
    {
        return FileParse::enumToString(type, WindowElementTypeStrings);
    }

    const std::array<std::string, 12> DeviceTypeStrings = {"Unknown",
                                                           "Venetian blind horizontal",
                                                           "Venetian blind vertical",
                                                           "Woven shade",
                                                           "Diffusing glass",
                                                           "Specular glass",
                                                           "Fritted Glass",
                                                           "Cellular Shade",
                                                           "Pleated Shade",
                                                           "Louvered Shutter",
                                                           "Solid Layer",
                                                           "Other"};

    DeviceType DeviceTypeFromString(std::string_view type)
    {
        return FileParse::enumFromString(type, DeviceTypeStrings, DeviceType::Unknown);
    }

    std::string DeviceTypeToString(DeviceType type)
    {
        return FileParse::enumToString(type, DeviceTypeStrings);
    }

    const std::array<std::string, 2> IncidentDataStructureStrings = {"Unknown", "Columns"};

    IncidentDataStructure IncidentDataStructureFromString(std::string_view type)
    {
        return FileParse::enumFromString(
          type, IncidentDataStructureStrings, IncidentDataStructure::Unknown);
    }

    std::string IncidentDataStructureToString(IncidentDataStructure type)
    {
        return FileParse::enumToString(type, IncidentDataStructureStrings);
    }

    const std::array<std::string, 7> WavelengthDataDirectionStrings = {"Unknown",
                                                                       "Transmission Front",
                                                                       "Transmission Back",
                                                                       "Reflection Front",
                                                                       "Reflection Back",
                                                                       "Absorbed Front",
                                                                       "Absorbed Back"};

    WavelengthDataDirection WavelengthDataDirectionFromString(std::string_view type)
    {
        return FileParse::enumFromString(
          type, WavelengthDataDirectionStrings, WavelengthDataDirection::Unknown);
    }

    std::string WavelengthDataDirectionToString(WavelengthDataDirection type)
    {
        return FileParse::enumToString(type, WavelengthDataDirectionStrings);
    }

    const std::array<std::string, 4> ScatteringDataTypeStrings = {
      "Unknown", "BTDF", "Fractional Diffuse", "Fractional Specular"};

    ScatteringDataType ScatteringDataTypeFromString(std::string_view type)
    {
        return FileParse::enumFromString(
          type, ScatteringDataTypeStrings, ScatteringDataType::Unknown);
    }

    std::string ScatteringDataTypeToString(ScatteringDataType type)
    {
        return FileParse::enumToString(type, ScatteringDataTypeStrings);
    }

    const std::array<std::string, 3> FileTypeStrings = {"Unknown", "BSDF", "NonBSDF"};

    FileType FileTypeFromString(std::string_view type)
    {
        return FileParse::enumFromString(type, FileTypeStrings, FileType::Unknown);
    }

    std::string FileTypeToString(FileType type)
    {
        return FileParse::enumToString(type, FileTypeStrings);
    }

    const std::array<std::string, 3> WavelengthUnitStrings = {"Unknown", "Integral", "Nanometer"};

    WavelengthUnit WavelengthUnitFromString(std::string_view type)
    {
        return FileParse::enumFromString(type, WavelengthUnitStrings, WavelengthUnit::Unknown);
    }

    std::string WavelengthUnitToString(WavelengthUnit type)
    {
        return FileParse::enumToString(type, WavelengthUnitStrings);
    }

    const std::array<std::string, 3> LengthUnitStrings = {"Unknown", "Meter", "Millimeter"};

    LengthUnit LengthUnitFromString(std::string_view type)
    {
        return FileParse::enumFromString(type, LengthUnitStrings, LengthUnit::Unknown);
    }

    std::string LengthUnitToString(LengthUnit type)
    {
        return FileParse::enumToString(type, LengthUnitStrings);
    }

    const std::array<std::string, 2> ConductivityUnitStrings = {"Unknown", "WPerMeterK"};

    ConductivityUnit ConductivityUnitFromString(std::string_view type)
    {
        return FileParse::enumFromString(type, ConductivityUnitStrings, ConductivityUnit::Unknown);
    }

    std::string ConductivityUnitToString(ConductivityUnit type)
    {
        return FileParse::enumToString(type, ConductivityUnitStrings);
    }
}   // namespace BSDFXML
