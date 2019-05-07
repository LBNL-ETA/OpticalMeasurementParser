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
        ProductData parseFile(const std::string & inputFile);
        ProductData parseJSONString(std::string const& json);
        ProductData parseJSONFile(std::string const & fname);
    private:
        void parseHeaderLine(const std::string & line, ProductData & product);
        void parseMeasurementLine(const std::string & line, ProductData & product);
        void parsePropertyAtTheEnd(const std::string & searchString,
                                   const std::string & behind,
                                   const std::string & line,
                                   double & property);
        void parseEmissivities(const std::string & line, ProductData & product);
        void parseUnits(const std::string & line, ProductData & product);
        void parseNFRCID(const std::string & line, ProductData & product);
        void parseStringPropertyInsideBraces(const std::string & line, std::string search, std::string & property);
    };

	ProductData parseFile(const std::string & inputFile);
    ProductData parseJSONString(std::string const & json);
    ProductData parseJSONFile(std::string const & fname);
}   // namespace OpticsParser
