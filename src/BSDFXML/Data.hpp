#pragma once

#include <string>
#include <optional>
#include <variant>
#include <vector>

#include "Enumerators.hpp"

namespace BSDFXML
{
    struct Thickness
    {
        double value;
        LengthUnit unit;
    };

    struct Wavelength
    {
        std::string value;
        WavelengthUnit unit;
        std::optional<std::string> type;
    };

    struct OpticalProperties
    {
        std::optional<double> transmittance;
        std::optional<double> reflectanceFront;
        std::optional<double> reflectanceBack;
        std::optional<std::string> comments;
    };

    struct Material
    {
        std::string name;
        std::optional<std::string> productName;
        std::optional<std::string> manufacturer;
        std::optional<Thickness> thickness;
        std::optional<DeviceType> deviceType;
        std::optional<double> thermalConductivity;
        std::optional<double> airPermeability;
        std::optional<double> emissivityFront;
        std::optional<double> emissivityBack;
        std::optional<double> TIR;
        std::optional<double> effectiveOpennessFraction;
        std::optional<double> permeabilityFactor;
        std::optional<OpticalProperties> opticalProperties;
        std::optional<std::string> color;
        std::optional<std::string> AERCAcceptance;
        std::optional<std::string> comments;
        std::optional<double> width;
        std::optional<double> height;
        std::optional<double> openness;
    };

    /// Schema is actually using many different types that look like this. I will not repeat them
    /// all here but will use single one. In case schema gets updated and one of the parameters is
    /// updated, then additional parameter can be added here.
    struct Length
    {
        double value;
        LengthUnit unit;
    };

    struct LengthWithCavity
    {
        double value;
        std::optional<int> cavity;
        LengthUnit unit;
    };

    struct Geometry
    {
        std::optional<std::string> format;
        std::optional<LengthUnit> unit;
        std::optional<Length> blindSlatThickness;
        std::optional<Length> blindCurvature;
        std::optional<Length> blindWidth;
        std::optional<Length> blindSpacing;
        std::optional<double> blindAngle;
        std::optional<double> diffusingGlassCoverageFraction;
        std::optional<double> wovenShadeOpennessFraction;
        std::optional<Length> wovenShadeThreadDiameter;
        std::optional<Length> wovenShadeThreadSpacing;
        std::optional<Length> wovenShadeThickness;
        std::optional<Length> cellularShadeCellHeight;
        std::optional<Length> cellularShadeInnerWallLength;
        std::optional<LengthWithCavity> cellularShadeSideWallLength;
        std::optional<LengthWithCavity> pleatedShadeCellHeight;
        std::optional<LengthWithCavity> pleatedShadeCellSideWallLength;
        std::optional<Length> mgfBlock;
    };

    struct ThetaBounds
    {
        std::optional<double> lowerTheta;
        std::optional<double> upperTheta;
        std::optional<std::string> comments;
    };

    struct AngleBasisBlock
    {
        double theta;
        std::optional<double> phi;
        std::optional<double> nPhis;
        std::optional<std::variant<double, ThetaBounds>> bounds;
        std::optional<std::string> comments;
    };

    struct AngleBasis
    {
        std::optional<std::string> name;
        std::vector<AngleBasisBlock> blocks;
    };

    struct DataDefinition
    {
        std::optional<IncidentDataStructure> incidentDataStructure;
        AngleBasis angleBasis;
        std::optional<std::string> comments;
    };

    using ScatteringData = std::vector<std::vector<double>>;

    struct WavelengthDataBlock
    {
        std::optional<WavelengthDataDirection> wavelengthDataDirection;
        std::optional<std::string> columnAngleBasis;
        std::optional<std::string> rowAngleBasis;
        std::optional<ScatteringDataType> scatteringDataType;
        ScatteringData scatteringData;
    };

    struct WavelengthData
    {
        std::optional<std::string> layerNumber;
        std::optional<std::string> angle;
        std::optional<Wavelength> wavelength;
        std::optional<std::string> sourceSpectrum;
        std::optional<std::string> detectorSpectrum;
        WavelengthDataBlock block;
        std::optional<std::string> comments;
    };

    struct Layer
    {
        std::optional<Material> material;
        std::optional<Geometry> geometry;
        std::optional<DataDefinition> dataDefinition;
        std::vector<WavelengthData> wavelengthData;
    };

    struct Optical
    {
        Layer layer;
    };

    struct WindowElement
    {
        WindowElementType windowElementType{WindowElementType::Unknown};
        std::optional<FileType> fileType;
        std::optional<std::string> Checksum;
        Optical optical;
    };
}   // namespace BSDFXML