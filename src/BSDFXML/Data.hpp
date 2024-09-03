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
        double Value;
        std::string unit;
    };

    struct OpticalProperties
    {
        std::optional<double> Transmittance;
        std::optional<double> ReflectanceFront;
        std::optional<double> ReflectanceBack;
        std::optional<std::string> Comments;
    };

    struct Material
    {
        std::string Name;
        std::optional<std::string> ProductName;
        std::optional<std::string> Manufacturer;
        std::optional<Thickness> Thickness;
        std::optional<DeviceType> DeviceType;
        std::optional<double> ThermalConductivity;
        std::optional<double> AirPermeability;
        std::optional<double> EmissivityFront;
        std::optional<double> EmissivityBack;
        std::optional<double> TIR;
        std::optional<double> EffectiveOpennessFraction;
        std::optional<double> PermeabilityFactor;
        std::optional<OpticalProperties> OpticalProperties;
        std::optional<std::string> Color;
        std::optional<std::string> AERCAcceptance;
        std::optional<std::string> Comments;
        std::optional<double> Width;
        std::optional<double> Height;
        std::optional<double> Openness;
    };

    struct ThetaBounds
    {
        std::optional<double> LowerTheta;
        std::optional<double> UpperTheta;
        std::optional<std::string> Comments;
    };

    struct AngleBasisBlock
    {
        double Theta;
        std::optional<double> Phi;
        std::optional<double> nPhis;
        std::optional<std::variant<double, ThetaBounds>> Bounds;
        std::optional<std::string> Comments;
    };

    struct AngleBasis
    {
        std::optional<std::string> Name;
        std::vector<AngleBasisBlock> Blocks;
    };

    struct DataDefinition
    {
        std::optional<IncidentDataStructure> IncidentDataStructure;
        std::vector<AngleBasis> AngleBasis;
        std::optional<std::string> Comments;
    };
}   // namespace BSDFXML