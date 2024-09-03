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
        node >> Child{"productName", material.productName};
        node >> Child{"Manufacturer", material.manufacturer};
        node >> Child{"Thickness", material.thickness};
        FileParse::deserializeEnum(
          node, "deviceType", material.deviceType, BSDFXML::DeviceTypeFromString);
        node >> Child{"thermalConductivity", material.thermalConductivity};
        node >> Child{"airPermeability", material.airPermeability};
        node >> Child{"emissivityFront", material.emissivityFront};
        node >> Child{"emissivityBack", material.emissivityBack};
        node >> Child{"TIR", material.TIR};
        node >> Child{"effectiveOpennessFraction", material.effectiveOpennessFraction};
        node >> Child{"permeabilityFactor", material.permeabilityFactor};
        node >> Child{"opticalProperties", material.opticalProperties};
        node >> Child{"color", material.color};
        node >> Child{"AERCAcceptance", material.AERCAcceptance};
        node >> Child{"comments", material.comments};
        node >> Child{"Width", material.width};
        node >> Child{"Height", material.height};
        node >> Child{"openness", material.openness};

        return node;
    }

    template<typename NodeAdapter>
    NodeAdapter & operator<<(NodeAdapter & node, const BSDFXML::Material & material)
    {
        using FileParse::Child;

        node << Child{"Name", material.name};
        node << Child{"productName", material.productName};
        node << Child{"Manufacturer", material.manufacturer};
        node << Child{"Thickness", material.thickness};
        FileParse::serializeEnum(
          node, "deviceType", material.deviceType, BSDFXML::DeviceTypeToString);
        node << Child{"thermalConductivity", material.thermalConductivity};
        node << Child{"airPermeability", material.airPermeability};
        node << Child{"emissivityFront", material.emissivityFront};
        node << Child{"emissivityBack", material.emissivityBack};
        node << Child{"TIR", material.TIR};
        node << Child{"effectiveOpennessFraction", material.effectiveOpennessFraction};
        node << Child{"permeabilityFactor", material.permeabilityFactor};
        node << Child{"opticalProperties", material.opticalProperties};
        node << Child{"color", material.color};
        node << Child{"AERCAcceptance", material.AERCAcceptance};
        node << Child{"comments", material.comments};
        node << Child{"Width", material.width};
        node << Child{"Height", material.height};
        node << Child{"openness", material.openness};

        return node;
    }

    template<typename NodeAdapter>
    const NodeAdapter & operator>>(const NodeAdapter & node, BSDFXML::ThetaBounds & thetaBounds)
    {
        node >> FileParse::Child{"lowerTheta", thetaBounds.lowerTheta};
        node >> FileParse::Child{"upperTheta", thetaBounds.upperTheta};
        node >> FileParse::Child{"comments", thetaBounds.comments};

        return node;
    }

    template<typename NodeAdapter>
    NodeAdapter & operator<<(NodeAdapter & node, const BSDFXML::ThetaBounds & thetaBounds)
    {
        node << FileParse::Child{"lowerTheta", thetaBounds.lowerTheta};
        node << FileParse::Child{"upperTheta", thetaBounds.upperTheta};
        node << FileParse::Child{"comments", thetaBounds.comments};

        return node;
    }

    template<typename NodeAdapter>
    const NodeAdapter & operator>>(const NodeAdapter & node,
                                   BSDFXML::AngleBasisBlock & angleBasisBlock)
    {
        node >> angleBasisBlock.theta;
        node >> FileParse::Child{"phi", angleBasisBlock.phi};
        node >> FileParse::Child{"nPhis", angleBasisBlock.nPhis};
        FileParse::deserializeOptionalVariant(
          node, {"SolidAngle", "ThetaBounds"}, angleBasisBlock.bounds);
        node >> FileParse::Child{"comments", angleBasisBlock.comments};

        return node;
    }

    template<typename NodeAdapter>
    NodeAdapter & operator<<(NodeAdapter & node, const BSDFXML::AngleBasisBlock & angleBasisBlock)
    {
        node << angleBasisBlock.theta;
        node << FileParse::Child{"phi", angleBasisBlock.phi};
        node << FileParse::Child{"nPhis", angleBasisBlock.nPhis};
        FileParse::serializeOptionalVariant(
          node, {"SolidAngle", "ThetaBounds"}, angleBasisBlock.bounds);
        node << FileParse::Child{"bounds", angleBasisBlock.bounds};
        node << FileParse::Child{"comments", angleBasisBlock.comments};

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
                                   "incidentDataStructure",
                                   dataDefinition.incidentDataStructure,
                                   BSDFXML::IncidentDataStructureFromString);
        node >> FileParse::Child{"angleBasis", dataDefinition.angleBasis};
        node >> FileParse::Child{"comments", dataDefinition.comments};

        return node;
    }

    template<typename NodeAdapter>
    NodeAdapter & operator<<(NodeAdapter & node, const BSDFXML::DataDefinition & dataDefinition)
    {
        FileParse::serializeEnum(node,
                                 "incidentDataStructure",
                                 dataDefinition.incidentDataStructure,
                                 BSDFXML::IncidentDataStructureToString);
        node << FileParse::Child{"angleBasis", dataDefinition.angleBasis};
        node << FileParse::Child{"comments", dataDefinition.comments};

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
        const NodeAdapter & operator>>(const NodeAdapter & node, BSDFXML::WavelengthData & wavelengthData)
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

}   // namespace BSDFXML