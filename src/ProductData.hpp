#pragma once

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

    struct ProductData;
    struct CompositionInformation
    {
        std::shared_ptr<ProductData> material;
        std::shared_ptr<ProductGeometry> geometry;
    };

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
        std::optional<std::string> productName;
        std::string productType;
        std::string manufacturer;
        std::optional<std::string> productSubtype;
        std::optional<int> nfrcid;
        std::optional<int> cgdbShadingLayerId;
        std::optional<int> cgdbShadeMaterialId;
        std::optional<double> thickness;
        std::optional<double> conductivity;
        std::optional<double> IRTransmittance;
        std::optional<double> frontEmissivity;
        std::optional<double> backEmissivity;
        std::optional<std::string> frontEmissivitySource;
        std::optional<std::string> backEmissivitySource;
        std::optional<std::string> material;
        std::optional<std::string> coatingName;
        std::optional<std::string> coatedSide;
        std::optional<std::string> substrateFilename;
        std::optional<std::string> appearance;
        std::optional<std::string> acceptance;
        std::optional<std::string> fileName;
        std::optional<std::string> unitSystem;
        std::optional<std::string> wavelengthUnit;
        std::optional<std::variant<std::vector<WLData>, DualBandBSDF>> measurements;
        std::optional<std::string> extrapolation;
        std::optional<int> aercID;
        std::optional<bool> specularity;
        std::optional<double> permeabilityFactor;
        std::optional<int> igdbChecksum;
        std::optional<std::string> igdbDatabaseVersion;
        std::optional<int> cgdbChecksum;
        std::optional<std::string> cgdbDatabaseVersion;
        std::optional<double> density;
        std::optional<double> youngsModulus;
        std::optional<std::string> dataFileName;
        std::optional<double> opticalOpenness;
        std::optional<DualBandValues> dualBandSpecular;
        std::optional<DualBandValues> dualBandDiffuse;
        std::optional<PrecalculatedResults> precalculatedResults;
        std::optional<PVPowerProperties> pvPowerProperties;
        std::optional<std::string> thicknessUnit;
        std::optional<CompositionInformation> composition;
    };

    // Converting to json requires updating and is not currently being
    // used so disabling for now.
    // void to_json(nlohmann::json & j, WLData const & wl);
    // void to_json(nlohmann::json & j, ProductData const & wl);


}   // namespace OpticsParser
