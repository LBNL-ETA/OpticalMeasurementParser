#pragma once

#include <fileParse/Enum.hxx>

#include "Enumerators.hpp"

namespace BSDFXML
{
    template<typename NodeAdapter>
    const NodeAdapter & operator>>(const NodeAdapter & node,
                                   BSDFXML::WindowElementType & windowElementType)
    {
        return FileParse::deserializeEnum<NodeAdapter, WindowElementType>(
          node, windowElementType, windowElementTypeFromString);
    }

    template<typename NodeAdapter>
    NodeAdapter & operator<<(NodeAdapter & node,
                             const BSDFXML::WindowElementType & windowElementType)
    {
        return FileParse::serializeEnum<NodeAdapter, WindowElementType>(
          node, windowElementType, windowElementTypeToString);
    }

    template<typename NodeAdapter>
    const NodeAdapter & operator>>(const NodeAdapter & node, BSDFXML::DeviceType & deviceType)
    {
        return FileParse::deserializeEnum<NodeAdapter, DeviceType>(
          node, deviceType, DeviceTypeFromString);
    }

    template<typename NodeAdapter>
    NodeAdapter & operator<<(NodeAdapter & node, const BSDFXML::DeviceType & deviceType)
    {
        return FileParse::serializeEnum<NodeAdapter, DeviceType>(
          node, deviceType, DeviceTypeToString);
    }

    template<typename NodeAdapter>
    const NodeAdapter & operator>>(const NodeAdapter & node,
                                   BSDFXML::IncidentDataStructure & incidentDataStructure)
    {
        return FileParse::deserializeEnum<NodeAdapter, IncidentDataStructure>(
          node, incidentDataStructure, IncidentDataStructureFromString);
    }

    template<typename NodeAdapter>
    NodeAdapter & operator<<(NodeAdapter & node,
                             const BSDFXML::IncidentDataStructure & incidentDataStructure)
    {
        return FileParse::serializeEnum<NodeAdapter, IncidentDataStructure>(
          node, incidentDataStructure, IncidentDataStructureToString);
    }

    template<typename NodeAdapter>
    const NodeAdapter & operator>>(const NodeAdapter & node,
                                   BSDFXML::WavelengthDataDirection & wavelengthDataDirection)
    {
        return FileParse::deserializeEnum<NodeAdapter, WavelengthDataDirection>(
          node, wavelengthDataDirection, WavelengthDataDirectionFromString);
    }

    template<typename NodeAdapter>
    NodeAdapter & operator<<(NodeAdapter & node,
                             const BSDFXML::WavelengthDataDirection & wavelengthDataDirection)
    {
        return FileParse::serializeEnum<NodeAdapter, WavelengthDataDirection>(
          node, wavelengthDataDirection, WavelengthDataDirectionToString);
    }

    template<typename NodeAdapter>
    const NodeAdapter & operator>>(const NodeAdapter & node,
                                   BSDFXML::ScatteringDataType & scatteringDataType)
    {
        return FileParse::deserializeEnum<NodeAdapter, ScatteringDataType>(
          node, scatteringDataType, ScatteringDataTypeFromString);
    }

    template<typename NodeAdapter>
    NodeAdapter & operator<<(NodeAdapter & node,
                             const BSDFXML::ScatteringDataType & scatteringDataType)
    {
        return FileParse::serializeEnum<NodeAdapter, ScatteringDataType>(
          node, scatteringDataType, ScatteringDataTypeToString);
    }

    template<typename NodeAdapter>
    const NodeAdapter & operator>>(const NodeAdapter & node, BSDFXML::FileType & fileType)
    {
        return FileParse::deserializeEnum<NodeAdapter, FileType>(
          node, fileType, FileTypeFromString);
    }

    template<typename NodeAdapter>
    NodeAdapter & operator<<(NodeAdapter & node, const BSDFXML::FileType & fileType)
    {
        return FileParse::serializeEnum<NodeAdapter, FileType>(node, fileType, FileTypeToString);
    }

    template<typename NodeAdapter>
    const NodeAdapter & operator>>(const NodeAdapter & node,
                                   BSDFXML::WavelengthUnit & wavelengthUnit)
    {
        return FileParse::deserializeEnum<NodeAdapter, WavelengthUnit>(
          node, wavelengthUnit, WavelengthUnitFromString);
    }

    template<typename NodeAdapter>
    NodeAdapter & operator<<(NodeAdapter & node, const BSDFXML::WavelengthUnit & wavelengthUnit)
    {
        return FileParse::serializeEnum<NodeAdapter, WavelengthUnit>(
          node, wavelengthUnit, WavelengthUnitToString);
    }

    template<typename NodeAdapter>
    const NodeAdapter & operator>>(const NodeAdapter & node, BSDFXML::LengthUnit & lengthUnit)
    {
        return FileParse::deserializeEnum<NodeAdapter, LengthUnit>(
          node, lengthUnit, LengthUnitFromString);
    }

    template<typename NodeAdapter>
    NodeAdapter & operator<<(NodeAdapter & node, const BSDFXML::LengthUnit & lengthUnit)
    {
        return FileParse::serializeEnum<NodeAdapter, LengthUnit>(
          node, lengthUnit, LengthUnitToString);
    }

    template<typename NodeAdapter>
    const NodeAdapter & operator>>(const NodeAdapter & node,
                                   BSDFXML::ConductivityUnit & conductivityUnit)
    {
        return FileParse::deserializeEnum<NodeAdapter, ConductivityUnit>(
          node, conductivityUnit, ConductivityUnitFromString);
    }

    template<typename NodeAdapter>
    NodeAdapter & operator<<(NodeAdapter & node, const BSDFXML::ConductivityUnit & conductivityUnit)
    {
        return FileParse::serializeEnum<NodeAdapter, ConductivityUnit>(
          node, conductivityUnit, ConductivityUnitToString);
    }
}   // namespace BSDFXML