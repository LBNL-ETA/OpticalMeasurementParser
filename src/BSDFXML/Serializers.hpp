#pragma once

#include <fileParse/Optional.hxx>
#include <fileParse/Variant.hxx>
#include <fileParse/Enum.hxx>
#include <fileParse/Attributes.hxx>
#include <fileParse/Vector.hxx>
#include <fileParse/StringConversion.hxx>

#include "Data.hpp"
#include "Enumerators.hpp"

namespace BSDFXML
{
    template<typename NodeAdapter>
    const NodeAdapter & operator>>(const NodeAdapter & node, BSDFXML::Thickness & thickness)
    {
        FileParse::loadAttribute(node, "unit", thickness.unit);
        node >> thickness.value;

        return node;
    }

    template<typename NodeAdapter>
    NodeAdapter & operator<<(NodeAdapter & node, const BSDFXML::Thickness & thickness)
    {
        FileParse::saveAttribute(node, "unit", thickness.unit);
        node << thickness.value;

        return node;
    }

    template<typename NodeAdapter>
    const NodeAdapter & operator>>(const NodeAdapter & node, BSDFXML::Wavelength & wavelength)
    {
        FileParse::loadAttribute(node, "unit", wavelength.unit);
        FileParse::loadAttribute(node, "type", wavelength.type);
        node >> wavelength.value;

        return node;
    }

    template<typename NodeAdapter>
    NodeAdapter & operator<<(NodeAdapter & node, const BSDFXML::Wavelength & wavelength)
    {
        FileParse::saveAttribute(node, "unit", wavelength.unit);
        FileParse::saveAttribute(node, "type", wavelength.type);
        node << wavelength.value;

        return node;
    }

    template<typename NodeAdapter>
    const NodeAdapter & operator>>(const NodeAdapter & node,
                                   BSDFXML::OpticalProperties & opticalProperties)
    {
        node >> FileParse::Child{"Transmittance", opticalProperties.transmittance};
        node >> FileParse::Child{"reflectanceFront", opticalProperties.reflectanceFront};
        node >> FileParse::Child{"reflectanceBack", opticalProperties.reflectanceBack};
        node >> FileParse::Child{"comments", opticalProperties.comments};

        return node;
    }

    template<typename NodeAdapter>
    NodeAdapter & operator<<(NodeAdapter & node,
                             const BSDFXML::OpticalProperties & opticalProperties)
    {
        node << FileParse::Child{"Transmittance", opticalProperties.transmittance};
        node << FileParse::Child{"reflectanceFront", opticalProperties.reflectanceFront};
        node << FileParse::Child{"reflectanceBack", opticalProperties.reflectanceBack};
        node << FileParse::Child{"comments", opticalProperties.comments};

        return node;
    }

    template<typename NodeAdapter>
    const NodeAdapter & operator>>(const NodeAdapter & node, BSDFXML::Material & material)
    {
        using FileParse::Child;
        node >> Child{"Name", material.name};
        node >> Child{"ProductName", material.productName};
        node >> Child{"Manufacturer", material.manufacturer};
        node >> Child{"Thickness", material.thickness};
        FileParse::deserializeEnum(
          node, "DeviceType", material.deviceType, BSDFXML::DeviceTypeFromString);
        node >> Child{"ThermalConductivity", material.thermalConductivity};
        node >> Child{"AirPermeability", material.airPermeability};
        node >> Child{"EmissivityFront", material.emissivityFront};
        node >> Child{"EmissivityBack", material.emissivityBack};
        node >> Child{"TIR", material.TIR};
        node >> Child{"EffectiveOpennessFraction", material.effectiveOpennessFraction};
        node >> Child{"PermeabilityFactor", material.permeabilityFactor};
        node >> Child{"OpticalProperties", material.opticalProperties};
        node >> Child{"Color", material.color};
        node >> Child{"AERCAcceptance", material.AERCAcceptance};
        node >> Child{"Comments", material.comments};
        node >> Child{"Width", material.width};
        node >> Child{"Height", material.height};
        node >> Child{"Openness", material.openness};

        return node;
    }

    template<typename NodeAdapter>
    NodeAdapter & operator<<(NodeAdapter & node, const BSDFXML::Material & material)
    {
        using FileParse::Child;

        node << Child{"Name", material.name};
        node << Child{"ProductName", material.productName};
        node << Child{"Manufacturer", material.manufacturer};
        node << Child{"Thickness", material.thickness};
        FileParse::serializeEnum(
          node, "DeviceType", material.deviceType, BSDFXML::DeviceTypeToString);
        node << Child{"ThermalConductivity", material.thermalConductivity};
        node << Child{"AirPermeability", material.airPermeability};
        node << Child{"EmissivityFront", material.emissivityFront};
        node << Child{"EmissivityBack", material.emissivityBack};
        node << Child{"TIR", material.TIR};
        node << Child{"EffectiveOpennessFraction", material.effectiveOpennessFraction};
        node << Child{"permeabilityFactor", material.permeabilityFactor};
        node << Child{"OpticalProperties", material.opticalProperties};
        node << Child{"Color", material.color};
        node << Child{"AERCAcceptance", material.AERCAcceptance};
        node << Child{"Comments", material.comments};
        node << Child{"Width", material.width};
        node << Child{"Height", material.height};
        node << Child{"Openness", material.openness};

        return node;
    }

    template<typename NodeAdapter>
    const NodeAdapter & operator>>(const NodeAdapter & node, BSDFXML::Length & length)
    {
        node >> length.value;
        FileParse::loadAttribute(node, "unit", length.unit, LengthUnitFromString);

        return node;
    }

    template<typename NodeAdapter>
    NodeAdapter & operator<<(NodeAdapter & node, const BSDFXML::Length & length)
    {
        node << length.value;
        FileParse::saveAttribute(node, "unit", length.unit, LengthUnitToString);

        return node;
    }

    template<typename NodeAdapter>
    const NodeAdapter & operator>>(const NodeAdapter & node,
                                   BSDFXML::LengthWithCavity & lengthWithCavity)
    {
        node >> lengthWithCavity.value;
        FileParse::loadAttribute(node, "cavity", lengthWithCavity.cavity);
        FileParse::loadAttribute(node, "unit", lengthWithCavity.unit, LengthUnitFromString);

        return node;
    }

    template<typename NodeAdapter>
    NodeAdapter & operator<<(NodeAdapter & node, const BSDFXML::LengthWithCavity & lengthWithCavity)
    {
        node << lengthWithCavity.value;
        FileParse::saveAttribute(node, "cavity", lengthWithCavity.cavity);
        FileParse::saveAttribute(node, "unit", lengthWithCavity.unit, LengthUnitToString);

        return node;
    }

    template<typename NodeAdapter>
    const NodeAdapter & operator>>(const NodeAdapter & node, BSDFXML::Geometry & geometry)
    {
        FileParse::loadAttribute(node, "unit", geometry.unit, LengthUnitFromString);
        FileParse::loadAttribute(node, "format", geometry.format);
        node >> FileParse::Child{"BlindSlatThickness", geometry.blindSlatThickness};
        node >> FileParse::Child{"BlindCurvature", geometry.blindCurvature};
        node >> FileParse::Child{"BlindWidth", geometry.blindWidth};
        node >> FileParse::Child{"BlindSpacing", geometry.blindSpacing};
        node >> FileParse::Child{"BlindAngle", geometry.blindAngle};
        node >> FileParse::Child{"DiffusingGlassCoverageFraction",
                                 geometry.diffusingGlassCoverageFraction};
        node >> FileParse::Child{"WovenShadeOpennessFraction", geometry.wovenShadeOpennessFraction};
        node >> FileParse::Child{"WovenShadeThreadDiameter", geometry.wovenShadeThreadDiameter};
        node >> FileParse::Child{"WovenShadeThreadSpacing", geometry.wovenShadeThreadSpacing};
        node >> FileParse::Child{"WovenShadeThickness", geometry.wovenShadeThickness};
        node >> FileParse::Child{"CellularShadeCellHeight", geometry.cellularShadeCellHeight};
        node >> FileParse::Child{"CellularShadeInnerWallLength",
                                 geometry.cellularShadeInnerWallLength};
        node
          >> FileParse::Child{"CellularShadeSideWallLength", geometry.cellularShadeSideWallLength};
        node >> FileParse::Child{"PleatedShadeCellHeight", geometry.pleatedShadeCellHeight};
        node >> FileParse::Child{"PleatedShadeCellSideWallLength",
                                 geometry.pleatedShadeCellSideWallLength};
        node >> FileParse::Child{"MgfBlock", geometry.mgfBlock};

        return node;
    }

    template<typename NodeAdapter>
    NodeAdapter & operator<<(NodeAdapter & node, const BSDFXML::Geometry & geometry)
    {
        FileParse::saveAttribute(node, "unit", geometry.unit, LengthUnitToString);
        FileParse::saveAttribute(node, "format", geometry.format);
        node << FileParse::Child{"BlindSlatThickness", geometry.blindSlatThickness};
        node << FileParse::Child{"BlindCurvature", geometry.blindCurvature};
        node << FileParse::Child{"BlindWidth", geometry.blindWidth};
        node << FileParse::Child{"BlindSpacing", geometry.blindSpacing};
        node << FileParse::Child{"BlindAngle", geometry.blindAngle};
        node << FileParse::Child{"DiffusingGlassCoverageFraction",
                                 geometry.diffusingGlassCoverageFraction};
        node << FileParse::Child{"WovenShadeOpennessFraction", geometry.wovenShadeOpennessFraction};
        node << FileParse::Child{"WovenShadeThreadDiameter", geometry.wovenShadeThreadDiameter};
        node << FileParse::Child{"WovenShadeThreadSpacing", geometry.wovenShadeThreadSpacing};
        node << FileParse::Child{"WovenShadeThickness", geometry.wovenShadeThickness};
        node << FileParse::Child{"CellularShadeCellHeight", geometry.cellularShadeCellHeight};
        node << FileParse::Child{"CellularShadeInnerWallLength",
                                 geometry.cellularShadeInnerWallLength};
        node << FileParse::Child{"CellularShadeSideWallLength",
                                 geometry.cellularShadeSideWallLength};
        node << FileParse::Child{"PleatedShadeCellHeight", geometry.pleatedShadeCellHeight};
        node << FileParse::Child{"PleatedShadeCellSideWallLength",
                                 geometry.pleatedShadeCellSideWallLength};
        node << FileParse::Child{"MgfBlock", geometry.mgfBlock};

        return node;
    }

    template<typename NodeAdapter>
    const NodeAdapter & operator>>(const NodeAdapter & node, BSDFXML::ThetaBounds & thetaBounds)
    {
        node >> FileParse::Child{"LowerTheta", thetaBounds.lowerTheta};
        node >> FileParse::Child{"UpperTheta", thetaBounds.upperTheta};
        node >> FileParse::Child{"Comments", thetaBounds.comments};

        return node;
    }

    template<typename NodeAdapter>
    NodeAdapter & operator<<(NodeAdapter & node, const BSDFXML::ThetaBounds & thetaBounds)
    {
        node << FileParse::Child{"LowerTheta", thetaBounds.lowerTheta};
        node << FileParse::Child{"UpperTheta", thetaBounds.upperTheta};
        node << FileParse::Child{"Comments", thetaBounds.comments};

        return node;
    }

    template<typename NodeAdapter>
    const NodeAdapter & operator>>(const NodeAdapter & node,
                                   BSDFXML::AngleBasisBlock & angleBasisBlock)
    {
        node >> angleBasisBlock.theta;
        node >> FileParse::Child{"Phi", angleBasisBlock.phi};
        node >> FileParse::Child{"nPhis", angleBasisBlock.nPhis};
        FileParse::deserializeOptionalVariant(
          node, {"SolidAngle", "ThetaBounds"}, angleBasisBlock.bounds);
        node >> FileParse::Child{"Comments", angleBasisBlock.comments};

        return node;
    }

    template<typename NodeAdapter>
    NodeAdapter & operator<<(NodeAdapter & node, const BSDFXML::AngleBasisBlock & angleBasisBlock)
    {
        node << angleBasisBlock.theta;
        node << FileParse::Child{"Phi", angleBasisBlock.phi};
        node << FileParse::Child{"nPhis", angleBasisBlock.nPhis};
        FileParse::serializeOptionalVariant(
          node, {"SolidAngle", "ThetaBounds"}, angleBasisBlock.bounds);
        node << FileParse::Child{"Bounds", angleBasisBlock.bounds};
        node << FileParse::Child{"Comments", angleBasisBlock.comments};

        return node;
    }

    template<typename NodeAdapter>
    const NodeAdapter & operator>>(const NodeAdapter & node, BSDFXML::AngleBasis & angleBasis)
    {
        node >> FileParse::Child{"Name", angleBasis.name};
        node >> FileParse::Child{"AngleBasisBlock", angleBasis.blocks};

        return node;
    }

    template<typename NodeAdapter>
    NodeAdapter & operator<<(NodeAdapter & node, const BSDFXML::AngleBasis & angleBasis)
    {
        node << FileParse::Child{"Name", angleBasis.name};
        node << FileParse::Child{"AngleBasisBlock", angleBasis.blocks};

        return node;
    }

    template<typename NodeAdapter>
    const NodeAdapter & operator>>(const NodeAdapter & node,
                                   BSDFXML::DataDefinition & dataDefinition)
    {
        FileParse::deserializeEnum(node,
                                   "IncidentDataStructure",
                                   dataDefinition.incidentDataStructure,
                                   BSDFXML::IncidentDataStructureFromString);
        node >> FileParse::Child{"AngleBasis", dataDefinition.angleBasis};
        node >> FileParse::Child{"Comments", dataDefinition.comments};

        return node;
    }

    template<typename NodeAdapter>
    NodeAdapter & operator<<(NodeAdapter & node, const BSDFXML::DataDefinition & dataDefinition)
    {
        FileParse::serializeEnum(node,
                                 "IncidentDataStructure",
                                 dataDefinition.incidentDataStructure,
                                 BSDFXML::IncidentDataStructureToString);
        node << FileParse::Child{"AngleBasis", dataDefinition.angleBasis};
        node << FileParse::Child{"Comments", dataDefinition.comments};

        return node;
    }

    namespace
    {
        /// Trims whitespace from the start and end of a string.
        inline std::string trimWhitespace(const std::string & str)
        {
            const auto start = str.find_first_not_of(" \t");
            const auto end = str.find_last_not_of(" \t");

            return (start == std::string::npos || end == std::string::npos)
                     ? ""
                     : str.substr(start, end - start + 1);
        }

        /// Parses a single line of comma-separated values into a vector of doubles.
        inline std::vector<double> parseRow(const std::string & line)
        {
            std::vector<double> row;
            std::istringstream lineStream(line);
            std::string cell;

            while(std::getline(lineStream, cell, ','))
            {
                cell = trimWhitespace(cell);

                if(!cell.empty())
                {
                    try
                    {
                        // Convert cell to double and add to row
                        row.push_back(FileParse::from_string<double>(cell));
                    }
                    catch(const std::exception &)
                    {
                        // Handle conversion error if necessary
                    }
                }
            }

            return row;
        }

        /// Parses a string representation of scattering data into a vector of vectors of doubles.
        inline BSDFXML::ScatteringData parseScatteringData(const std::string & value)
        {
            BSDFXML::ScatteringData scatteringData;
            std::istringstream inputStream(value);
            std::string line;

            while(std::getline(inputStream, line))
            {
                auto row = parseRow(line);
                if(!row.empty())
                {
                    scatteringData.push_back(row);
                }
            }

            return scatteringData;
        }
    }   // namespace

    template<typename NodeAdapter>
    const NodeAdapter & operator>>(const NodeAdapter & node,
                                   BSDFXML::ScatteringData & scatteringData)
    {
        std::string value;
        node >> FileParse::Child{"ScatteringData", value};

        scatteringData = parseScatteringData(value);

        return node;
    }

    namespace
    {
        /// Converts a row of double values into a comma-separated string.
        std::string serializeRow(const std::vector<double> & row)
        {
            std::ostringstream rowStream;

            for(std::size_t i = 0; i < row.size(); ++i)
            {
                rowStream << row[i];
                if(i < row.size() - 1)   // Add a comma between elements, but not after the last one
                {
                    rowStream << ", ";
                }
            }

            return rowStream.str();
        }

        /// Serializes the entire ScatteringData into a single string.
        std::string serializeScatteringData(const BSDFXML::ScatteringData & scatteringData)
        {
            std::ostringstream outputStream;

            for(const auto & row : scatteringData)
            {
                outputStream << serializeRow(row) << "\n";   // Serialize each row and add a newline
            }

            return outputStream.str();
        }
    }   // anonymous namespace

    template<typename NodeAdapter>
    NodeAdapter & operator<<(NodeAdapter & node, const BSDFXML::ScatteringData & scatteringData)
    {
        const std::string value{serializeScatteringData(scatteringData)};
        node << FileParse::Child{"ScatteringData", value};

        return node;
    }

    template<typename NodeAdapter>
    const NodeAdapter & operator>>(const NodeAdapter & node,
                                   BSDFXML::WavelengthDataBlock & wavelengthDataBlock)
    {
        FileParse::deserializeEnum(node,
                                   "WavelengthDataDirection",
                                   wavelengthDataBlock.wavelengthDataDirection,
                                   BSDFXML::WavelengthDataDirectionFromString);
        node >> FileParse::Child{"ColumnAngleBasis", wavelengthDataBlock.columnAngleBasis};
        node >> FileParse::Child{"RowAngleBasis", wavelengthDataBlock.rowAngleBasis};
        FileParse::deserializeEnum(node,
                                   "ScatteringDataType",
                                   wavelengthDataBlock.scatteringDataType,
                                   BSDFXML::ScatteringDataTypeFromString);
        node >> FileParse::Child{"ScatteringData", wavelengthDataBlock.scatteringData};

        return node;
    }

    template<typename NodeAdapter>
    NodeAdapter & operator<<(NodeAdapter & node,
                             const BSDFXML::WavelengthDataBlock & wavelengthDataBlock)
    {
        FileParse::serializeEnum(node,
                                 "WavelengthDataDirection",
                                 wavelengthDataBlock.wavelengthDataDirection,
                                 BSDFXML::WavelengthDataDirectionToString);
        node << FileParse::Child{"ColumnAngleBasis", wavelengthDataBlock.columnAngleBasis};
        node << FileParse::Child{"RowAngleBasis", wavelengthDataBlock.rowAngleBasis};
        FileParse::serializeEnum(node,
                                 "ScatteringDataType",
                                 wavelengthDataBlock.scatteringDataType,
                                 BSDFXML::ScatteringDataTypeToString);
        node << FileParse::Child{"ScatteringData", wavelengthDataBlock.scatteringData};

        return node;
    }

    template<typename NodeAdapter>
    const NodeAdapter & operator>>(const NodeAdapter & node,
                                   BSDFXML::WavelengthData & wavelengthData)
    {
        node >> FileParse::Child{"LayerNumber", wavelengthData.layerNumber};
        node >> FileParse::Child{"Angle", wavelengthData.angle};
        node >> FileParse::Child{"Wavelength", wavelengthData.wavelength};
        node >> FileParse::Child{"SourceSpectrum", wavelengthData.sourceSpectrum};
        node >> FileParse::Child{"DetectorSpectrum", wavelengthData.detectorSpectrum};
        node >> FileParse::Child{"WavelengthDataBlock", wavelengthData.blocks};
        node >> FileParse::Child{"Comments", wavelengthData.comments};

        return node;
    }

    template<typename NodeAdapter>
    NodeAdapter & operator<<(NodeAdapter & node, const BSDFXML::WavelengthData & wavelengthData)
    {
        node << FileParse::Child{"LayerNumber", wavelengthData.layerNumber};
        node << FileParse::Child{"Angle", wavelengthData.angle};
        node << FileParse::Child{"Wavelength", wavelengthData.wavelength};
        node << FileParse::Child{"SourceSpectrum", wavelengthData.sourceSpectrum};
        node << FileParse::Child{"DetectorSpectrum", wavelengthData.detectorSpectrum};
        node << FileParse::Child{"WavelengthDataBlock", wavelengthData.blocks};
        node << FileParse::Child{"Comments", wavelengthData.comments};

        return node;
    }

}   // namespace BSDFXML