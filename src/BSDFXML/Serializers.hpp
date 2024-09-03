#pragma once

#include <fileParse/Optional.hxx>
#include <fileParse/Variant.hxx>
#include <fileParse/Enum.hxx>
#include <fileParse/Attributes.hxx>
#include <fileParse/Vector.hxx>

#include "Data.hpp"
#include "Enumerators.hpp"

namespace BSDFXML
{
    template<typename NodeAdapter>
    const NodeAdapter & operator>>(const NodeAdapter & node, BSDFXML::Thickness & thickness)
    {
        FileParse::loadAttribute(node, "unit", thickness.unit);
        node >> thickness.Value;

        return node;
    }

    template<typename NodeAdapter>
    NodeAdapter & operator<<(NodeAdapter & node, const BSDFXML::Thickness & thickness)
    {
        FileParse::saveAttribute(node, "unit", thickness.unit);
        node << thickness.Value;

        return node;
    }

    template<typename NodeAdapter>
    const NodeAdapter & operator>>(const NodeAdapter & node,
                                   BSDFXML::OpticalProperties & opticalProperties)
    {
        node >> FileParse::Child{"Transmittance", opticalProperties.Transmittance};
        node >> FileParse::Child{"ReflectanceFront", opticalProperties.ReflectanceFront};
        node >> FileParse::Child{"ReflectanceBack", opticalProperties.ReflectanceBack};
        node >> FileParse::Child{"Comments", opticalProperties.Comments};

        return node;
    }

    template<typename NodeAdapter>
    NodeAdapter & operator<<(NodeAdapter & node,
                             const BSDFXML::OpticalProperties & opticalProperties)
    {
        node << FileParse::Child{"Transmittance", opticalProperties.Transmittance};
        node << FileParse::Child{"ReflectanceFront", opticalProperties.ReflectanceFront};
        node << FileParse::Child{"ReflectanceBack", opticalProperties.ReflectanceBack};
        node << FileParse::Child{"Comments", opticalProperties.Comments};

        return node;
    }

    template<typename NodeAdapter>
    const NodeAdapter & operator>>(const NodeAdapter & node, BSDFXML::Material & material)
    {
        using FileParse::Child;
        node >> Child{"Name", material.Name};
        node >> Child{"ProductName", material.ProductName};
        node >> Child{"Manufacturer", material.Manufacturer};
        node >> Child{"Thickness", material.Thickness};
        FileParse::deserializeEnum(
          node, "DeviceType", material.DeviceType, BSDFXML::DeviceTypeFromString);
        node >> Child{"ThermalConductivity", material.ThermalConductivity};
        node >> Child{"AirPermeability", material.AirPermeability};
        node >> Child{"EmissivityFront", material.EmissivityFront};
        node >> Child{"EmissivityBack", material.EmissivityBack};
        node >> Child{"TIR", material.TIR};
        node >> Child{"EffectiveOpennessFraction", material.EffectiveOpennessFraction};
        node >> Child{"PermeabilityFactor", material.PermeabilityFactor};
        node >> Child{"OpticalProperties", material.OpticalProperties};
        node >> Child{"Color", material.Color};
        node >> Child{"AERCAcceptance", material.AERCAcceptance};
        node >> Child{"Comments", material.Comments};
        node >> Child{"Width", material.Width};
        node >> Child{"Height", material.Height};
        node >> Child{"Openness", material.Openness};

        return node;
    }

    template<typename NodeAdapter>
    NodeAdapter & operator<<(NodeAdapter & node, const BSDFXML::Material & material)
    {
        using FileParse::Child;

        node << Child{"Name", material.Name};
        node << Child{"ProductName", material.ProductName};
        node << Child{"Manufacturer", material.Manufacturer};
        node << Child{"Thickness", material.Thickness};
        FileParse::serializeEnum(
          node, "DeviceType", material.DeviceType, BSDFXML::DeviceTypeToString);
        node << Child{"ThermalConductivity", material.ThermalConductivity};
        node << Child{"AirPermeability", material.AirPermeability};
        node << Child{"EmissivityFront", material.EmissivityFront};
        node << Child{"EmissivityBack", material.EmissivityBack};
        node << Child{"TIR", material.TIR};
        node << Child{"EffectiveOpennessFraction", material.EffectiveOpennessFraction};
        node << Child{"PermeabilityFactor", material.PermeabilityFactor};
        node << Child{"OpticalProperties", material.OpticalProperties};
        node << Child{"Color", material.Color};
        node << Child{"AERCAcceptance", material.AERCAcceptance};
        node << Child{"Comments", material.Comments};
        node << Child{"Width", material.Width};
        node << Child{"Height", material.Height};
        node << Child{"Openness", material.Openness};

        return node;
    }

    template<typename NodeAdapter>
    const NodeAdapter & operator>>(const NodeAdapter & node, BSDFXML::ThetaBounds & thetaBounds)
    {
        node >> FileParse::Child{"LowerTheta", thetaBounds.LowerTheta};
        node >> FileParse::Child{"UpperTheta", thetaBounds.UpperTheta};
        node >> FileParse::Child{"Comments", thetaBounds.Comments};

        return node;
    }

    template<typename NodeAdapter>
    NodeAdapter & operator<<(NodeAdapter & node, const BSDFXML::ThetaBounds & thetaBounds)
    {
        node << FileParse::Child{"LowerTheta", thetaBounds.LowerTheta};
        node << FileParse::Child{"UpperTheta", thetaBounds.UpperTheta};
        node << FileParse::Child{"Comments", thetaBounds.Comments};

        return node;
    }

    template<typename NodeAdapter>
    const NodeAdapter & operator>>(const NodeAdapter & node,
                                   BSDFXML::AngleBasisBlock & angleBasisBlock)
    {
        node >> angleBasisBlock.Theta;
        node >> FileParse::Child{"Phi", angleBasisBlock.Phi};
        node >> FileParse::Child{"nPhis", angleBasisBlock.nPhis};
        FileParse::deserializeOptionalVariant(
          node, {"SolidAngle", "ThetaBounds"}, angleBasisBlock.Bounds);
        node >> FileParse::Child{"Comments", angleBasisBlock.Comments};

        return node;
    }

    template<typename NodeAdapter>
    NodeAdapter & operator<<(NodeAdapter & node, const BSDFXML::AngleBasisBlock & angleBasisBlock)
    {
        node << angleBasisBlock.Theta;
        node << FileParse::Child{"Phi", angleBasisBlock.Phi};
        node << FileParse::Child{"nPhis", angleBasisBlock.nPhis};
        FileParse::serializeOptionalVariant(
          node, {"SolidAngle", "ThetaBounds"}, angleBasisBlock.Bounds);
        node << FileParse::Child{"Bounds", angleBasisBlock.Bounds};
        node << FileParse::Child{"Comments", angleBasisBlock.Comments};

        return node;
    }

    template<typename NodeAdapter>
    const NodeAdapter & operator>>(const NodeAdapter & node, BSDFXML::AngleBasis & angleBasis)
    {
        node >> FileParse::Child{"Name", angleBasis.Name};
        node >> angleBasis.Blocks;

        return node;
    }

    template<typename NodeAdapter>
    NodeAdapter & operator<<(NodeAdapter & node, const BSDFXML::AngleBasis & angleBasis)
    {
        node << FileParse::Child{"Name", angleBasis.Name};
        node << angleBasis.Blocks;

        return node;
    }

}   // namespace BSDFXML