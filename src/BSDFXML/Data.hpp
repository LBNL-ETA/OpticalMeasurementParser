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
        std::string unit;
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
        std::vector<AngleBasis> angleBasis;
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
}   // namespace BSDFXML