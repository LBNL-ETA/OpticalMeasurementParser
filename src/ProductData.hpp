#pragma once

#include <vector>
#include <string>
#include <optional>

#include <nlohmann/json.hpp>

namespace OpticsParser
{
    struct WLData
    {
    public:
        WLData(double wavelength, double T, double frontR, double backR);

        double wavelength;
        double T;
        double frontR;
        double backR;
    };

    struct ProductData
    {
        ProductData() = default;
        ProductData(std::string const & productName,
                    std::string const & productType,
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
        std::optional<int> nfrcid;
        double thickness;
        std::optional<double> conductivity;
        double IRTransmittance;
        double frontEmissivity;
        double backEmissivity;
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
        std::vector<WLData> measurements;
    };

    void to_json(nlohmann::json & j, WLData const & wl);
    void to_json(nlohmann::json & j, ProductData const & wl);
}   // namespace OpticsParser
