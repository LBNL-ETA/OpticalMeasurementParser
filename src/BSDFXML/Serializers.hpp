#pragma once

#include <algorithm>
#include <regex>
#include <iterator>

#include <fileParse/Common.hxx>
#include <fileParse/Optional.hxx>
#include <fileParse/Variant.hxx>
#include <fileParse/Attributes.hxx>
#include <fileParse/Vector.hxx>
#include <fileParse/StringConversion.hxx>

#include "Data.hpp"
#include "Enumerators.hpp"

#include "EnumSerializers.hpp"

namespace BSDFXML
{
    template<typename NodeAdapter>
    const NodeAdapter & operator>>(const NodeAdapter & node, BSDFXML::Thickness & thickness)
    {
        using FileParse::operator>>;   // operators for basic C++ types are in the FileParse
                                       // namespace

        FileParse::loadAttribute<NodeAdapter, BSDFXML::LengthUnit>(
          node, "unit", thickness.unit, BSDFXML::LengthUnitFromString);
        node >> thickness.value;

        return node;
    }

    template<typename NodeAdapter>
    NodeAdapter & operator<<(NodeAdapter & node, const BSDFXML::Thickness & thickness)
    {
        using FileParse::operator<<;   // operators for basic C++ types are in the FileParse
                                       // namespace

        FileParse::saveAttribute<NodeAdapter, BSDFXML::LengthUnit>(
          node, "unit", thickness.unit, LengthUnitToString);
        node << thickness.value;

        return node;
    }

    template<typename NodeAdapter>
    const NodeAdapter & operator>>(const NodeAdapter & node, BSDFXML::Wavelength & wavelength)
    {
        using FileParse::operator>>;   // operators for basic C++ types are in the FileParse
                                       // namespace

        FileParse::loadAttribute<NodeAdapter, BSDFXML::WavelengthUnit>(
          node, "unit", wavelength.unit, WavelengthUnitFromString);
        FileParse::loadAttribute(node, "type", wavelength.type);
        node >> wavelength.value;

        return node;
    }

    template<typename NodeAdapter>
    NodeAdapter & operator<<(NodeAdapter & node, const BSDFXML::Wavelength & wavelength)
    {
        using FileParse::operator<<;

        FileParse::saveAttribute<NodeAdapter, BSDFXML::WavelengthUnit>(
          node, "unit", wavelength.unit, WavelengthUnitToString);
        FileParse::saveAttribute(node, "type", wavelength.type);
        node << wavelength.value;

        return node;
    }

    template<typename NodeAdapter>
    const NodeAdapter & operator>>(const NodeAdapter & node,
                                   BSDFXML::OpticalProperties & opticalProperties)
    {
        node >> FileParse::Child{"Transmittance", opticalProperties.transmittance};
        node >> FileParse::Child{"ReflectanceFront", opticalProperties.reflectanceFront};
        node >> FileParse::Child{"ReflectanceBack", opticalProperties.reflectanceBack};
        node >> FileParse::Child{"Comments", opticalProperties.comments};

        return node;
    }

    template<typename NodeAdapter>
    NodeAdapter & operator<<(NodeAdapter & node,
                             const BSDFXML::OpticalProperties & opticalProperties)
    {
        node << FileParse::Child{"Transmittance", opticalProperties.transmittance};
        node << FileParse::Child{"ReflectanceFront", opticalProperties.reflectanceFront};
        node << FileParse::Child{"ReflectanceBack", opticalProperties.reflectanceBack};
        node << FileParse::Child{"Comments", opticalProperties.comments};

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
        node >> Child{"DeviceType", material.deviceType};
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
        node << Child{"DeviceType", material.deviceType};
        node << Child{"ThermalConductivity", material.thermalConductivity};
        node << Child{"AirPermeability", material.airPermeability};
        node << Child{"EmissivityFront", material.emissivityFront};
        node << Child{"EmissivityBack", material.emissivityBack};
        node << Child{"TIR", material.TIR};
        node << Child{"EffectiveOpennessFraction", material.effectiveOpennessFraction};
        node << Child{"PermeabilityFactor", material.permeabilityFactor};
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
        using FileParse::operator>>;   // operators for basic C++ types are in the FileParse
                                       // namespace

        node >> length.value;
        FileParse::loadAttribute<NodeAdapter, BSDFXML::LengthUnit>(
          node, "unit", length.unit, LengthUnitFromString);

        return node;
    }

    template<typename NodeAdapter>
    NodeAdapter & operator<<(NodeAdapter & node, const BSDFXML::Length & length)
    {
        using FileParse::operator<<;   // operators for basic C++ types are in the FileParse
                                       // namespace

        node << length.value;
        FileParse::saveAttribute<NodeAdapter, BSDFXML::LengthUnit>(
          node, "unit", length.unit, LengthUnitToString);

        return node;
    }

    template<typename NodeAdapter>
    const NodeAdapter & operator>>(const NodeAdapter & node,
                                   BSDFXML::LengthWithCavity & lengthWithCavity)
    {
        using FileParse::operator>>;   // operators for basic C++ types are in the FileParse
                                       // namespace

        node >> lengthWithCavity.value;
        FileParse::loadAttribute(node, "cavity", lengthWithCavity.cavity);
        FileParse::loadAttribute<NodeAdapter, BSDFXML::LengthUnit>(
          node, "unit", lengthWithCavity.unit, LengthUnitFromString);

        return node;
    }

    template<typename NodeAdapter>
    NodeAdapter & operator<<(NodeAdapter & node, const BSDFXML::LengthWithCavity & lengthWithCavity)
    {
        using FileParse::operator<<;   // operators for basic C++ types are in the FileParse
                                       // namespace

        node << lengthWithCavity.value;
        FileParse::saveAttribute(node, "cavity", lengthWithCavity.cavity);
        FileParse::saveAttribute<NodeAdapter, BSDFXML::LengthUnit>(
          node, "unit", lengthWithCavity.unit, LengthUnitToString);

        return node;
    }

    template<typename NodeAdapter>
    const NodeAdapter & operator>>(const NodeAdapter & node, BSDFXML::Geometry & geometry)
    {
        FileParse::loadAttribute<NodeAdapter, LengthUnit>(
          node, "unit", geometry.unit, LengthUnitFromString);
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
        FileParse::saveAttribute<NodeAdapter, LengthUnit>(
          node, "unit", geometry.unit, LengthUnitToString);
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
        node >> FileParse::Child{"Theta", angleBasisBlock.theta};
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
        node << FileParse::Child{"Theta", angleBasisBlock.theta};
        node << FileParse::Child{"Phi", angleBasisBlock.phi};
        node << FileParse::Child{"nPhis", angleBasisBlock.nPhis};
        FileParse::serializeOptionalVariant(
          node, {"SolidAngle", "ThetaBounds"}, angleBasisBlock.bounds);
        node << FileParse::Child{"Comments", angleBasisBlock.comments};

        return node;
    }

    template<typename NodeAdapter>
    const NodeAdapter & operator>>(const NodeAdapter & node, BSDFXML::AngleBasis & angleBasis)
    {
        node >> FileParse::Child{"AngleBasisName", angleBasis.name};
        node >> FileParse::Child{"AngleBasisBlock", angleBasis.blocks};

        return node;
    }

    template<typename NodeAdapter>
    NodeAdapter & operator<<(NodeAdapter & node, const BSDFXML::AngleBasis & angleBasis)
    {
        node << FileParse::Child{"AngleBasisName", angleBasis.name};
        node << FileParse::Child{"AngleBasisBlock", angleBasis.blocks};

        return node;
    }

    template<typename NodeAdapter>
    const NodeAdapter & operator>>(const NodeAdapter & node,
                                   BSDFXML::DataDefinition & dataDefinition)
    {
        node >> FileParse::Child{"IncidentDataStructure", dataDefinition.incidentDataStructure};
        node >> FileParse::Child{"AngleBasis", dataDefinition.angleBasis};
        node >> FileParse::Child{"Comments", dataDefinition.comments};

        return node;
    }

    template<typename NodeAdapter>
    NodeAdapter & operator<<(NodeAdapter & node, const BSDFXML::DataDefinition & dataDefinition)
    {
        node << FileParse::Child{"IncidentDataStructure", dataDefinition.incidentDataStructure};
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
        std::vector<double> parseRow(const std::string & line)
        {
            std::vector<double> result;
            std::regex regex("[,\t]");   // Regular expression to match commas or tabs
            std::sregex_token_iterator iter(line.begin(), line.end(), regex, -1);
            std::sregex_token_iterator end;

            for(; iter != end; ++iter)
            {
                try
                {
                    if(!iter->str().empty())
                    {
                        result.push_back(std::stod(iter->str()));
                    }
                }
                catch(const std::invalid_argument &)
                {
                    // Handle conversion error if needed
                }
            }

            return result;
        }
    }   // namespace

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
            std::copy(row.begin(), row.end(), std::ostream_iterator<double>(rowStream, ", "));
            return rowStream.str();
        }
    }   // anonymous namespace

    /// Serializes the entire ScatteringData into a single string.
    inline std::string serializeScatteringData(const BSDFXML::ScatteringData & scatteringData)
    {
        std::ostringstream outputStream;

        // Adding new line at the beginning of the matrix so that it looks nicely formatted
        outputStream << "\n";

        std::for_each(scatteringData.begin(), scatteringData.end(), [&](const auto & row) {
            outputStream << serializeRow(row) << "\n";
        });

        return outputStream.str();
    }

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
        node >> FileParse::Child{"WavelengthDataDirection",
                                 wavelengthDataBlock.wavelengthDataDirection};
        node >> FileParse::Child{"ColumnAngleBasis", wavelengthDataBlock.columnAngleBasis};
        node >> FileParse::Child{"RowAngleBasis", wavelengthDataBlock.rowAngleBasis};
        node >> FileParse::Child{"ScatteringDataType", wavelengthDataBlock.scatteringDataType};
        // Child node name is moved down to the ScatteringData serializer since compiler cannot
        // resolve between serializer of std::vector<std::vector<double>> and Child<std::vector<T>>
        // defined in FileParse
        node >> wavelengthDataBlock.scatteringData;

        return node;
    }

    template<typename NodeAdapter>
    NodeAdapter & operator<<(NodeAdapter & node,
                             const BSDFXML::WavelengthDataBlock & wavelengthDataBlock)
    {
        node << FileParse::Child{"WavelengthDataDirection",
                                 wavelengthDataBlock.wavelengthDataDirection};
        node << FileParse::Child{"ColumnAngleBasis", wavelengthDataBlock.columnAngleBasis};
        node << FileParse::Child{"RowAngleBasis", wavelengthDataBlock.rowAngleBasis};
        node << FileParse::Child{"ScatteringDataType", wavelengthDataBlock.scatteringDataType};
        // Child node name is moved down to the ScatteringData serializer since compiler cannot
        // resolve between serializer of std::vector<std::vector<double>> and Child<std::vector<T>>
        // defined in FileParse
        node << wavelengthDataBlock.scatteringData;

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
        node >> FileParse::Child{"WavelengthDataBlock", wavelengthData.block};
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
        node << FileParse::Child{"WavelengthDataBlock", wavelengthData.block};
        node << FileParse::Child{"Comments", wavelengthData.comments};

        return node;
    }

    template<typename NodeAdapter>
    const NodeAdapter & operator>>(const NodeAdapter & node, BSDFXML::Layer & layer)
    {
        node >> FileParse::Child{"Material", layer.material};
        node >> FileParse::Child{"Geometry", layer.geometry};
        node >> FileParse::Child{"DataDefinition", layer.dataDefinition};
        node >> FileParse::Child{"WavelengthData", layer.wavelengthData};

        return node;
    }

    template<typename NodeAdapter>
    NodeAdapter & operator<<(NodeAdapter & node, const BSDFXML::Layer & layer)
    {
        node << FileParse::Child{"Material", layer.material};
        node << FileParse::Child{"Geometry", layer.geometry};
        node << FileParse::Child{"DataDefinition", layer.dataDefinition};
        node << FileParse::Child{"WavelengthData", layer.wavelengthData};

        return node;
    }

    template<typename NodeAdapter>
    const NodeAdapter & operator>>(const NodeAdapter & node, BSDFXML::Optical & optical)
    {
        node >> FileParse::Child{"Layer", optical.layer};

        return node;
    }

    template<typename NodeAdapter>
    NodeAdapter & operator<<(NodeAdapter & node, const BSDFXML::Optical & optical)
    {
        node << FileParse::Child{"Layer", optical.layer};

        return node;
    }

    template<typename NodeAdapter>
    const NodeAdapter & operator>>(const NodeAdapter & node, BSDFXML::WindowElement & windowElement)
    {
        node >> FileParse::Child{"WindowElementType", windowElement.windowElementType};
        node >> FileParse::Child{"FileType", windowElement.fileType};
        node >> FileParse::Child{"Checksum", windowElement.Checksum};
        node >> FileParse::Child{"Optical", windowElement.optical};

        return node;
    }

    template<typename NodeAdapter>
    NodeAdapter & operator<<(NodeAdapter & node, const BSDFXML::WindowElement & windowElement)
    {
        node << FileParse::Child{"WindowElementType", windowElement.windowElementType};
        node << FileParse::Child{"FileType", windowElement.fileType};
        node << FileParse::Child{"Checksum", windowElement.Checksum};
        node << FileParse::Child{"Optical", windowElement.optical};

        return node;
    }

}   // namespace BSDFXML