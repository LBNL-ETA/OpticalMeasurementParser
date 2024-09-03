#include <array>

#include <fileParse/StringConversion.hxx>

#include "Enumerators.hpp"

namespace BSDFXML
{
    const std::array<std::string, 4> WindowElementTypeStrings = {
      "Unknown", "System", "Layer", "Material"};

    WindowElementType windowElementFromString(std::string_view type)
    {
        return FileParse::enumFromString(
          type, WindowElementTypeStrings, WindowElementType::Unknown);
    }

    std::string windowElementToString(WindowElementType type)
    {
        return FileParse::enumToString(type, WindowElementTypeStrings);
    }

    const std::array<std::string, 11> DeviceTypeStrings = {"Unknown",
                                                           "Venetian blind horizontal",
                                                           "Venetian blind vertical",
                                                           "Woven shade",
                                                           "Diffusing glass",
                                                           "Specular glass",
                                                           "Fritted Glass",
                                                           "Cellular Shade",
                                                           "Pleated Shade",
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
}   // namespace BSDFXML
