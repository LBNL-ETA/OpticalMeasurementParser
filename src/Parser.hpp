#pragma once

#include <string>
#include <vector>
#include <ostream>
#include <limits>

namespace OpticsParser
{
    struct WLData
    {
    public:
        WLData(double wavelength, double T, double frontR, double backR);
        friend std::ostream & operator<<(std::ostream & os, const WLData & data);

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
        double conductivity = std::numeric_limits<double>::quiet_NaN();
        double IRTransmittance = std::numeric_limits<double>::quiet_NaN();
        double frontEmissivity = std::numeric_limits<double>::quiet_NaN();
        double backEmissivity = std::numeric_limits<double>::quiet_NaN();                
        std::vector<WLData> measurements;
    };
    class Parser
    {
    public:
        explicit Parser(const std::string & inputFile);
        friend std::ostream & operator<<(std::ostream & os, const Parser & parser);

        double thickness() const;
        double conductivity() const;
        double IRTransmittance() const;
        double frontEmissivity() const;
        double backEmissivity() const;
        int nfrcid() const;
        const std::string & productName() const;
        const std::string & productType() const;
        std::vector<WLData> measurements() const;

    private:
        void parseHeaderLine(const std::string & line);
        void parseMeasurementLine(const std::string & line);
        void parsePropertyAtTheEnd(const std::string & searchString,
                                   const std::string & behind,
                                   const std::string & line,
                                   double & property);
        void parseEmissivities(const std::string & line);
        void parseNFRCID(const std::string & line);
        void parseProductName(const std::string & line);
        void parseProductType(const std::string & line);
        double m_Thickness;
        double m_Conductivity;
        double m_IRTransmittance;
        double m_FrontEmissivity;
        double m_BackEmissivity;
        int m_NFRCID;
        std::string m_ProductName;
        std::string m_Type;
        std::vector<WLData> m_WLData;
    };

	ProductData parseFile(std::string const& fname);

}   // namespace OpticsParser
