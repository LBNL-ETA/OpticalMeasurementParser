#pragma once

#include <vector>
#include <string>
#include <optional>

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

    struct WLData
    {
    public:
        WLData(double wavelength,
               MeasurementComponent directComponent,
               std::optional<MeasurementComponent> diffuseComponent =
                 std::optional<MeasurementComponent>());

        WLData(double wavelength, double tDirect, double rfDirect, double rbDiffuse);
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
        MeasurementComponent directComponent;
        std::optional<MeasurementComponent> diffuseComponent;
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
                         int numberSegments);

        double slatWidth;
        double slatSpacing;
        double slatCurvature;
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
        PerforatedGeometry(double spacingX, double spacingY, double dimensionX, double dimensionY,
                           std::string perforationType);
        double spacingX;
        double spacingY;
        double dimensionX;
        double dimensionY;
        std::string perforationType;
    };

    struct ProductData;

    struct CompositionInformation
    {
        std::shared_ptr<ProductData> material;
        std::shared_ptr<ProductGeometry> geometry;
    };

    struct ProductData
    {
        ProductData() = default;
        ProductData(ProductData const & other) = default;
        virtual ~ProductData() = default;
        ProductData(std::string const & productName,
                    std::string const & productType,
                    std::string const & subtype,
                    int nfrcid,
                    double thickness,
                    double conductivity,
                    double IRTransmittance,
                    double frontEmissivity,
                    double backEmissivity,
                    std::string frontEmissivitySource,
                    std::string backEmissivitySource,
                    std::string manufacturer,
                    std::string material,
                    std::string coatingName,
                    std::string coatedSide,
                    std::string substrateFilename,
                    std::string appearance,
                    std::string acceptance,
                    std::string fileName,
                    std::string unitSystem,
                    std::string wavelengthUnit,
                    std::vector<WLData> const & measurements);

        std::string productName;
        std::string productType;
        std::optional<std::string> subtype;
        std::optional<int> nfrcid;
        std::optional<double> thickness;
        std::optional<double> conductivity;
        std::optional<double> IRTransmittance;
        std::optional<double> frontEmissivity;
        std::optional<double> backEmissivity;
        std::optional<std::string> frontEmissivitySource;
        std::optional<std::string> backEmissivitySource;
        std::string manufacturer;
        std::optional<std::string> material;
        std::optional<std::string> coatingName;
        std::optional<std::string> coatedSide;
        std::optional<std::string> substrateFilename;
        std::optional<std::string> appearance;
        std::optional<std::string> acceptance;
        std::optional<std::string> fileName;
        std::optional<std::string> unitSystem;
        std::optional<std::string> wavelengthUnit;
        std::optional<std::vector<WLData>> measurements;
        std::optional<std::string> extrapolation;
        std::optional<int> aercID;
        std::optional<bool> specularity;
        std::optional<double> permeabilityFactor;
    };

    void to_json(nlohmann::json & j, WLData const & wl);
    void to_json(nlohmann::json & j, ProductData const & wl);

    struct ComposedProductData : ProductData
    {
        ComposedProductData(ProductData const & product,
                            std::shared_ptr<CompositionInformation> composition);

        std::shared_ptr<CompositionInformation> compositionInformation;
    };
}   // namespace OpticsParser
