#pragma once

#include "bsdfdata/Data.hpp"


#include <vector>
#include <string>
#include <optional>
#include <map>
#include <variant>
#include <nlohmann/json.hpp>

namespace OpticsParser
{
    struct MeasurementComponent
    {
        double tf;
        double tb;
        double rf;
        double rb;
    };

    struct PVWavelengthData
    {
        double eqef;
        double eqeb;
    };

    struct WLData
    {
    public:
        WLData(
          double wavelength,
          std::optional<MeasurementComponent> const & directComponent =
            std::optional<MeasurementComponent>(),
          std::optional<MeasurementComponent> const & diffuseComponent =
            std::optional<MeasurementComponent>(),
          std::optional<PVWavelengthData> const & pvComponent = std::optional<PVWavelengthData>());

        WLData(double wavelength, double tDirect, double rfDirect, double rbDirect);
        WLData(double wavelength,
               double tfDirect,
               double tfDiffuse,
               double tbDirect,
               double tbDiffuse,
               double rfDirect,
               double rfDiffuse,
               double rbDirect,
               double rbDiffuse);

        double wavelength;
        std::optional<MeasurementComponent> directComponent;
        std::optional<MeasurementComponent> diffuseComponent;
        std::optional<PVWavelengthData> pvComponent;
    };

    struct BSDF
    {
        std::vector<std::vector<double>> data;
        std::string rowAngleBasisName;
        std::string columnAngleBasisName;
    };

    struct WavelengthBSDFs
    {
        BSDF tf;
        BSDF tb;
        BSDF rf;
        BSDF rb;
    };

    struct DualBandBSDF
    {
        WavelengthBSDFs solar;
        WavelengthBSDFs visible;
    };

    struct ProductGeometry
    {
        ProductGeometry() = default;
        virtual ~ProductGeometry() = default;
    };

    struct VenetianGeometry : ProductGeometry
    {
        VenetianGeometry(double slatWidth,
                         double slatSpacing,
                         double slatCurvature,
                         double slatTilt = 0,
                         std::string const & tiltChoice = "0",
                         int numberSegments = 5);

        double slatWidth;
        double slatSpacing;
        double slatCurvature;
        double slatTilt;
        std::string tiltChoice;
        int numberSegments;
    };

    struct WovenGeometry : ProductGeometry
    {
        WovenGeometry(double threadDiameter, double threadSpacing, double shadeThickness);
        double threadDiameter;
        double threadSpacing;
        double shadeThickness;
    };

    struct PerforatedGeometry : ProductGeometry
    {
        PerforatedGeometry(double spacingX,
                           double spacingY,
                           double dimensionX,
                           double dimensionY,
                           std::string perforationType);
        double spacingX;
        double spacingY;
        double dimensionX;
        double dimensionY;
        std::string perforationType;
    };

    struct LouveredGeometry : ProductGeometry
    {
        LouveredGeometry(double slat_width,
                         double slat_thickness,
                         double slat_spacing,
                         double slat_angle);

        double SlatWidth;
        double SlatThickness;
        double SlatSpacing;
        double SlatAngle;
    };

    struct DualBandValues
    {
        std::optional<double> solarTransmittanceFront;
        std::optional<double> solarTransmittanceBack;
        std::optional<double> solarReflectanceFront;
        std::optional<double> solarReflectanceBack;
        std::optional<double> visibleTransmittanceFront;
        std::optional<double> visibleTransmittanceBack;
        std::optional<double> visibleReflectanceFront;
        std::optional<double> visibleReflectanceBack;
    };

    struct CIEValue
    {
        double x;
        double y;
        double z;
    };

    struct PrecalculatedResults
    {
        std::optional<double> solarTransmittanceFront;
        std::optional<double> solarReflectanceFront;
        std::optional<double> solarReflectanceBack;
        std::optional<double> visibleTransmittanceFront;
        std::optional<double> visibleReflectanceFront;
        std::optional<double> visibleReflectanceBack;
        std::optional<double> dwTransmittance;
        std::optional<double> spfTransmittance;
        std::optional<double> uvTransmittance;
        std::optional<CIEValue> cieTransmittance;
        std::optional<CIEValue> cieReflectanceFront;
    };

    struct PVPowerProperty
    {
        double jsc;
        double voc;
        double ff;
    };

    using PVPowerProperties = std::map<double, std::vector<PVPowerProperty>>;

    struct ProductData : std::enable_shared_from_this<ProductData>
    {
        ProductData() = default;
        ProductData(ProductData const & other) = default;
        virtual ~ProductData() = default;
        ProductData(std::string const & productName,
                    std::string const & productType,
                    std::string const & manufacturer);
        ProductData(std::string const & productName,
                    std::string const & productType,
                    std::string const & productSubtype,
                    std::string const & manufacturer);

        std::string name;
        std::optional<std::string> productName{std::nullopt};
        std::string productType;
        std::string manufacturer;
        std::optional<std::string> productSubtype{std::nullopt};
        std::optional<int> nfrcid{std::nullopt};
        std::optional<int> cgdbShadingLayerId{std::nullopt};
        std::optional<int> cgdbShadeMaterialId{std::nullopt};
        std::optional<double> thickness{std::nullopt};
        std::optional<double> conductivity{std::nullopt};
        std::optional<double> IRTransmittance{std::nullopt};
        std::optional<double> frontEmissivity{std::nullopt};
        std::optional<double> backEmissivity{std::nullopt};
        std::optional<std::string> frontEmissivitySource{std::nullopt};
        std::optional<std::string> backEmissivitySource{std::nullopt};
        std::optional<std::string> material{std::nullopt};
        std::optional<std::string> coatingName{std::nullopt};
        std::optional<std::string> coatedSide{std::nullopt};
        std::optional<std::string> substrateFilename{std::nullopt};
        std::optional<std::string> appearance{std::nullopt};
        std::optional<std::string> acceptance{std::nullopt};
        std::optional<std::string> fileName{std::nullopt};
        std::optional<std::string> unitSystem{std::nullopt};
        std::optional<std::string> wavelengthUnit{std::nullopt};
        std::optional<std::variant<std::vector<WLData>, DualBandBSDF>> measurements{std::nullopt};
        std::optional<std::string> extrapolation{std::nullopt};
        std::optional<int> aercID{std::nullopt};
        std::optional<bool> specularity{std::nullopt};
        std::optional<double> permeabilityFactor{std::nullopt};
        std::optional<int> igdbChecksum{std::nullopt};
        std::optional<std::string> igdbDatabaseVersion{std::nullopt};
        std::optional<int> cgdbChecksum{std::nullopt};
        std::optional<std::string> cgdbDatabaseVersion{std::nullopt};
        std::optional<double> density{std::nullopt};
        std::optional<double> youngsModulus{std::nullopt};
        std::optional<std::string> dataFileName{std::nullopt};
        std::optional<double> opticalOpenness{std::nullopt};
        std::optional<DualBandValues> dualBandSpecular{std::nullopt};
        std::optional<DualBandValues> dualBandDiffuse{std::nullopt};
        std::optional<PrecalculatedResults> precalculatedResults{std::nullopt};
        std::optional<PVPowerProperties> pvPowerProperties{std::nullopt};
        std::optional<std::string> thicknessUnit{std::nullopt};
        std::shared_ptr<ProductData> materialDefinition{nullptr};
        std::shared_ptr<ProductGeometry> geometry{nullptr};
        std::optional<BSDFData::DeviceType> deviceType{std::nullopt};
    };

    // Converting to json requires updating and is not currently being
    // used so disabling for now.
    // void to_json(nlohmann::json & j, WLData const & wl);
    // void to_json(nlohmann::json & j, ProductData const & wl);


}   // namespace OpticsParser
