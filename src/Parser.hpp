#pragma once

#include <string>
#include <vector>
#include <ostream>
#include <optional>

#include "ProductData.hpp"

namespace OpticsParser
{
    class Parser
    {
    public:
        ProductData parseFile(const std::string & inputFile);
        ProductData parseJSONString(std::string const & json);
        ProductData parseJSONFile(std::string const & fname);

    private:
        void parseHeaderLine(const std::string & line, ProductData & product);
        void parseMeasurementLine(const std::string & line, ProductData & product);
        void parseEmissivities(const std::string & line, ProductData & product);
        void parseUnits(const std::string & line, ProductData & product);
        void parseNFRCID(const std::string & line, ProductData & product);
        void parseAERCID(const std::string & line, ProductData & product);

		void parseBoolPropertyInsideBraces(const std::string & line,
                                           std::string search,
                                           std::optional<bool> & property);

		void parseDoublePropertyInsideBraces(const std::string & line,
                                                   std::string search,
                                                   std::optional<double> & property);


        template<typename T>
        void parsePropertyAtTheEnd(const std::string & searchString,
                                   const std::string & behind,
                                   const std::string & line,
                                   T & property)
        {
            if(line.find(searchString) != std::string::npos)
            {
                std::string str = line.substr(line.find(behind) + behind.size());
                property = std::stod(str);
            }
        }

        template<typename T>
        void parseStringPropertyInsideBraces(const std::string & line,
                                             std::string search,
                                             T & property)
        {
            search += ":";
            auto itr = line.find(search);
            if(itr != std::string::npos)
            {
                std::string str = line.substr(itr + search.size() + 1);
                auto erasePos = str.find('}');
                str.erase(erasePos, 1);
                // Removes all spaces from the beginning of the string
                while(str.size() && isspace(str.front()))
                    str.erase(str.begin());
                // Remove all spaces from the end of the string.
                while(str.size() && isspace(str.back()))
                    str.pop_back();
                property = str;
            }
        }
    };

    ProductData parseFile(const std::string & inputFile);
    ProductData parseJSONString(std::string const & json);
    ProductData parseJSONFile(std::string const & fname);
}   // namespace OpticsParser
