#pragma once

#include <string>
#include <vector>
#include <ostream>

#include "ProductData.hpp"

namespace OpticsParser
{
      
    class Parser
    {
    public:
        //explicit Parser(const std::string & inputFile);

	ProductData parse_file(const std::string & inputFile);

/*
        double thickness() const;
        double conductivity() const;
        double IRTransmittance() const;
        double frontEmissivity() const;
        double backEmissivity() const;
	std::string frontEmissivitySource() const;
	std::string backEmissivitySource() const;
        int nfrcid() const;
        std::string productName() const;
        std::string productType() const;
        std::vector<WLData> measurements() const;
        std::string manufacturer() const;
        std::string material() const;
        std::string coatingName() const;
        std::string coatedSide() const;
        std::string substrateFilename() const;
        std::string appearance() const;
        std::string acceptance() const;
        std::string fileName() const;
*/
    private:
        void parseHeaderLine(const std::string & line, ProductData & product);
        void parseMeasurementLine(const std::string & line, ProductData & product);
        void parsePropertyAtTheEnd(const std::string & searchString,
                                   const std::string & behind,
                                   const std::string & line,
                                   double & property);
        void parseEmissivities(const std::string & line, ProductData & product);
        void parseNFRCID(const std::string & line, ProductData & product);
//        void parseProductName(const std::string & line);
//        void parseProductType(const std::string & line);
	void parseStringPropertyInsideBraces(const std::string & line, std::string search, std::string & property);
        //ProductData 
/*
        double m_Thickness;
        double m_Conductivity;
        double m_IRTransmittance;
        double m_FrontEmissivity;
        double m_BackEmissivity;
        std::string m_FrontEmissivitySource;
        std::string m_BackEmissivitySource;
        int m_NFRCID;
        std::string m_ProductName;
        std::string m_Type;
        std::string m_Manufacturer;
        std::string m_Material;
        std::string m_CoatingName;
        std::string m_CoatedSide;
        std::string m_SubstrateFilename;
        std::string m_Appearance;
        std::string m_Acceptance;
        std::vector<WLData> m_WLData;
*/
    };

    ProductData parseFile(std::string const& fname);
    ProductData parseJSONString(std::string const& json);
    ProductData parseJSONFile(std::string const & fname);
}   // namespace OpticsParser
