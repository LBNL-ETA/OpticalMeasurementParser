#pragma once

#include <vector>
#include <string>

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
                    std::vector<WLData> const & measurements);

        std::string productName;
        std::string productType;
        int nfrcid;
        double thickness;
        double conductivity;
        double IRTransmittance;
        double frontEmissivity;
        double backEmissivity;
        std::vector<WLData> measurements;
    };
}   // namespace OpticsParser
