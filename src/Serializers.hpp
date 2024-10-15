

#pragma once

#include "fileParse/Optional.hxx"
#include "fileParse/Vector.hxx"
#include "fileParse/Variant.hxx"

#include "ProductData.hpp"

namespace OpticsParser
{
    template<typename NodeAdapter>
    NodeAdapter & operator<<(NodeAdapter & node, const MeasurementComponent & component)
    {
        node << FileParse::Child{"tf", component.tf};
        node << FileParse::Child{"rf", component.rf};
        node << FileParse::Child{"tb", component.tb};
        node << FileParse::Child{"rb", component.rb};
    }

    template<typename NodeAdapter>
    const NodeAdapter & operator>>(const NodeAdapter & node, MeasurementComponent & component)
    {
        node >> FileParse::Child{"tf", component.tf};
        node >> FileParse::Child{"rf", component.rf};
        node >> FileParse::Child{"tb", component.tb};
        node >> FileParse::Child{"rb", component.rb};
    }

    template<typename NodeAdapter>
    NodeAdapter & operator<<(NodeAdapter & node, const PVWavelengthData & pvData)
    {
        node << FileParse::Child{"eqef", pvData.eqef};
        node << FileParse::Child{"eqeb", pvData.eqeb};
    }

    template<typename NodeAdapter>
    const NodeAdapter & operator>>(const NodeAdapter & node, PVWavelengthData & pvData)
    {
        node >> FileParse::Child{"eqef", pvData.eqef};
        node >> FileParse::Child{"eqeb", pvData.eqeb};
    }

    template<typename NodeAdapter>
    NodeAdapter & operator<<(NodeAdapter & node, const WLData & wlData)
    {
        node << FileParse::Child{"wavelength", wlData.wavelength};
        node << FileParse::Child{"direct", wlData.directComponent};
        node << FileParse::Child{"diffuse", wlData.diffuseComponent};
        node << FileParse::Child{"pv", wlData.pvComponent};
    }

    template<typename NodeAdapter>
    const NodeAdapter & operator>>(const NodeAdapter & node, WLData & wlData)
    {
        node >> FileParse::Child{"wavelength", wlData.wavelength};
        node >> FileParse::Child{"direct", wlData.directComponent};
        node >> FileParse::Child{"diffuse", wlData.diffuseComponent};
        node >> FileParse::Child{"pv", wlData.pvComponent};
    }

    // TODO: Here we need a parser of comma separated values into std::vector<std::vector<double>>
    // This can be confirmed once we have test for it

    template<typename NodeAdapter>
    NodeAdapter & operator<<(NodeAdapter & node, const BSDF & bsdf)
    {
        node << FileParse::Child{"data", bsdf.data};   // This is in comma separated format
        node << FileParse::Child{"rowAngleBasisName", bsdf.rowAngleBasisName};
        node << FileParse::Child{"columnAngleBasisName", bsdf.columnAngleBasisName};
    }

    template<typename NodeAdapter>
    const NodeAdapter & operator>>(const NodeAdapter & node, BSDF & bsdf)
    {
        node >> FileParse::Child{"data", bsdf.data};   // This is in comma separated format
        node >> FileParse::Child{"rowAngleBasisName", bsdf.rowAngleBasisName};
        node >> FileParse::Child{"columnAngleBasisName", bsdf.columnAngleBasisName};
    }

    template<typename NodeAdapter>
    NodeAdapter & operator<<(NodeAdapter & node, const WavelengthBSDFs & bsdfs)
    {
        node << FileParse::Child{"Tf", bsdfs.tf};
        node << FileParse::Child{"Tb", bsdfs.tb};
        node << FileParse::Child{"Rf", bsdfs.rf};
        node << FileParse::Child{"Rb", bsdfs.rb};
    }

    template<typename NodeAdapter>
    const NodeAdapter & operator>>(const NodeAdapter & node, WavelengthBSDFs & bsdfs)
    {
        node >> FileParse::Child{"Tf", bsdfs.tf};
        node >> FileParse::Child{"Tb", bsdfs.tb};
        node >> FileParse::Child{"Rf", bsdfs.rf};
        node >> FileParse::Child{"Rb", bsdfs.rb};
    }

    template<typename NodeAdapter>
    NodeAdapter & operator<<(NodeAdapter & node, const DualBandBSDF & bsdf)
    {
        node << FileParse::Child{"Solar", bsdf.solar};
        node << FileParse::Child{"Visible", bsdf.visible};
    }

    template<typename NodeAdapter>
    const NodeAdapter & operator>>(const NodeAdapter & node, DualBandBSDF & bsdf)
    {
        node >> FileParse::Child{"Solar", bsdf.solar};
        node >> FileParse::Child{"Visible", bsdf.visible};
    }

    template<typename NodeAdapter>
    NodeAdapter & operator<<(NodeAdapter & node, const VenetianGeometry & venetian)
    {
        node << FileParse::Child{"SlatWidth", venetian.slatWidth};
        node << FileParse::Child{"SlatSpacing", venetian.slatSpacing};
        node << FileParse::Child{"SlatCurvature", venetian.slatCurvature};
        node << FileParse::Child{"SlatTilt", venetian.slatTilt};
        node << FileParse::Child{"TiltChoice", venetian.tiltChoice};
        node << FileParse::Child{"NumberSegments", venetian.numberSegments};
    }

    template<typename NodeAdapter>
    const NodeAdapter & operator>>(const NodeAdapter & node, VenetianGeometry & venetian)
    {
        node >> FileParse::Child{"SlatWidth", venetian.slatWidth};
        node >> FileParse::Child{"SlatSpacing", venetian.slatSpacing};
        node >> FileParse::Child{"SlatCurvature", venetian.slatCurvature};
        node >> FileParse::Child{"SlatTilt", venetian.slatTilt};
        node >> FileParse::Child{"TiltChoice", venetian.tiltChoice};
        node >> FileParse::Child{"NumberSegments", venetian.numberSegments};
    }

    template<typename NodeAdapter>
    NodeAdapter & operator<<(NodeAdapter & node, const WovenGeometry & woven)
    {
        node << FileParse::Child{"ThreadDiameter", woven.threadDiameter};
        node << FileParse::Child{"ThreadSpacing", woven.threadSpacing};
        node << FileParse::Child{"ShadeThickness", woven.shadeThickness};
    }

    template<typename NodeAdapter>
    const NodeAdapter & operator>>(const NodeAdapter & node, WovenGeometry & woven)
    {
        node >> FileParse::Child{"ThreadDiameter", woven.threadDiameter};
        node >> FileParse::Child{"ThreadSpacing", woven.threadSpacing};
        node >> FileParse::Child{"ShadeThickness", woven.shadeThickness};
    }

    template<typename NodeAdapter>
    NodeAdapter & operator<<(NodeAdapter & node, const PerforatedGeometry & perforated)
    {
        node << FileParse::Child{"SpacingX", perforated.spacingX};
        node << FileParse::Child{"SpacingY", perforated.spacingY};
        node << FileParse::Child{"DimensionX", perforated.dimensionX};
        node << FileParse::Child{"DimensionY", perforated.dimensionY};
        node << FileParse::Child{"PerforationType", perforated.perforationType};
    }

    template<typename NodeAdapter>
    const NodeAdapter & operator>>(const NodeAdapter & node, PerforatedGeometry & perforated)
    {
        node >> FileParse::Child{"SpacingX", perforated.spacingX};
        node >> FileParse::Child{"SpacingY", perforated.spacingY};
        node >> FileParse::Child{"DimensionX", perforated.dimensionX};
        node >> FileParse::Child{"DimensionY", perforated.dimensionY};
        node >> FileParse::Child{"PerforationType", perforated.perforationType};
    }

    template<typename NodeAdapter>
    NodeAdapter & operator<<(NodeAdapter & node, const DualBandValues & dualBandValues)
    {
        node << FileParse::Child{"SolarTransmittanceFront", dualBandValues.solarTransmittanceFront};
        node << FileParse::Child{"SolarTransmittanceBack", dualBandValues.solarTransmittanceBack};
        node << FileParse::Child{"SolarReflectanceFront", dualBandValues.solarReflectanceFront};
        node << FileParse::Child{"SolarReflectanceBack", dualBandValues.solarReflectanceBack};
        node << FileParse::Child{"VisibleTransmittanceFront",
                                 dualBandValues.visibleTransmittanceFront};
        node << FileParse::Child{"VisibleTransmittanceBack",
                                 dualBandValues.visibleTransmittanceBack};
        node << FileParse::Child{"VisibleReflectanceFront", dualBandValues.visibleReflectanceFront};
        node << FileParse::Child{"VisibleReflectanceBack", dualBandValues.visibleReflectanceBack};
    }

    template<typename NodeAdapter>
    const NodeAdapter & operator>>(const NodeAdapter & node, DualBandValues & dualBandValues)
    {
        node >> FileParse::Child{"SolarTransmittanceFront", dualBandValues.solarTransmittanceFront};
        node >> FileParse::Child{"SolarTransmittanceBack", dualBandValues.solarTransmittanceBack};
        node >> FileParse::Child{"SolarReflectanceFront", dualBandValues.solarReflectanceFront};
        node >> FileParse::Child{"SolarReflectanceBack", dualBandValues.solarReflectanceBack};
        node >> FileParse::Child{"VisibleTransmittanceFront",
                                 dualBandValues.visibleTransmittanceFront};
        node
          >> FileParse::Child{"VisibleTransmittanceBack", dualBandValues.visibleTransmittanceBack};
        node >> FileParse::Child{"VisibleReflectanceFront", dualBandValues.visibleReflectanceFront};
        node >> FileParse::Child{"VisibleReflectanceBack", dualBandValues.visibleReflectanceBack};
    }

    template<typename NodeAdapter>
    NodeAdapter & operator<<(NodeAdapter & node, const CIEValue & cieValue)
    {
        node << FileParse::Child{"x", cieValue.x};
        node << FileParse::Child{"y", cieValue.y};
        node << FileParse::Child{"z", cieValue.z};
    }

    template<typename NodeAdapter>
    const NodeAdapter & operator>>(const NodeAdapter & node, CIEValue & cieValue)
    {
        node >> FileParse::Child{"x", cieValue.x};
        node >> FileParse::Child{"y", cieValue.y};
        node >> FileParse::Child{"z", cieValue.z};
    }

    template<typename NodeAdapter>
    NodeAdapter & operator<<(NodeAdapter & node, const PrecalculatedResults & results)
    {
        node << FileParse::Child{"SolarTransmittanceFront", results.solarTransmittanceFront};
        node << FileParse::Child{"SolarReflectanceFront", results.solarReflectanceFront};
        node << FileParse::Child{"SolarReflectanceBack", results.solarReflectanceBack};
        node << FileParse::Child{"VisibleTransmittanceFront", results.visibleTransmittanceFront};
        node << FileParse::Child{"VisibleReflectanceFront", results.visibleReflectanceFront};
        node << FileParse::Child{"VisibleReflectanceBack", results.visibleReflectanceBack};
        node << FileParse::Child{"DWTransmittance", results.dwTransmittance};
        node << FileParse::Child{"SPFTransmittance", results.spfTransmittance};
        node << FileParse::Child{"UVTransmittance", results.uvTransmittance};
        node << FileParse::Child{"CIETransmittance", results.cieTransmittance};
        node << FileParse::Child{"CIEReflectanceFront", results.cieReflectanceFront};
    }

    template<typename NodeAdapter>
    const NodeAdapter & operator>>(const NodeAdapter & node, PrecalculatedResults & results)
    {
        node >> FileParse::Child{"SolarTransmittanceFront", results.solarTransmittanceFront};
        node >> FileParse::Child{"SolarReflectanceFront", results.solarReflectanceFront};
        node >> FileParse::Child{"SolarReflectanceBack", results.solarReflectanceBack};
        node >> FileParse::Child{"VisibleTransmittanceFront", results.visibleTransmittanceFront};
        node >> FileParse::Child{"VisibleReflectanceFront", results.visibleReflectanceFront};
        node >> FileParse::Child{"VisibleReflectanceBack", results.visibleReflectanceBack};
        node >> FileParse::Child{"DWTransmittance", results.dwTransmittance};
        node >> FileParse::Child{"SPFTransmittance", results.spfTransmittance};
        node >> FileParse::Child{"UVTransmittance", results.uvTransmittance};
        node >> FileParse::Child{"CIETransmittance", results.cieTransmittance};
        node >> FileParse::Child{"CIEReflectanceFront", results.cieReflectanceFront};
    }

    template<typename NodeAdapter>
    NodeAdapter & operator<<(NodeAdapter & node, const PVPowerProperty & pvPower)
    {
        node << FileParse::Child{"jsc", pvPower.jsc};
        node << FileParse::Child{"voc", pvPower.voc};
        node << FileParse::Child{"ff", pvPower.ff};
    }

    template<typename NodeAdapter>
    const NodeAdapter & operator>>(const NodeAdapter & node, PVPowerProperty & pvPower)
    {
        node >> FileParse::Child{"jsc", pvPower.jsc};
        node >> FileParse::Child{"voc", pvPower.voc};
        node >> FileParse::Child{"ff", pvPower.ff};
    }
}   // namespace OpticsParser