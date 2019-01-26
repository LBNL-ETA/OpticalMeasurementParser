#include "Parser.hpp"

#include <fstream>
#include <sstream>
#include <iostream>

std::ostream & OpticsParser::operator<<(std::ostream & os, const OpticsParser::WLData & data)
{
    os << data.wavelength << ", " << data.T << ", " << data.frontR << ", " << data.backR
       << std::endl;
    return os;
}

OpticsParser::Parser::Parser(const std::string & inputFile) :
    m_Thickness{-1},
    m_Conductivity{-1},
    m_IRTransmittance{-1},
    m_FrontEmissivity{-1},
    m_BackEmissivity{-1},
    m_NFRCID{-1},
    m_ProductName{""},
    m_Type{""}
{
    std::ifstream inFile(inputFile);
    std::string line;
    while(std::getline(inFile, line))
    {
        if(line.find('{') != std::string::npos)
        {
            parseHeaderLine(line);
        }
        else
        {
            if(!line.empty())
            {
                parseMeasurementLine(line);
            }
        }
    }
}

void OpticsParser::Parser::parseHeaderLine(const std::string & line)
{
    parsePropertyAtTheEnd("Thickness", "}", line, m_Thickness);
    parsePropertyAtTheEnd("Conductivity", "}", line, m_Conductivity);
    parsePropertyAtTheEnd("IR Transmittance", "TIR=", line, m_IRTransmittance);
    parseEmissivities(line);
    parseProductName(line);
    parseProductType(line);
    parseNFRCID(line);
}

void OpticsParser::Parser::parseMeasurementLine(const std::string & line)
{
    std::vector<double> result;
    std::istringstream iss(line);
    for(std::string s; iss >> s;)
        result.push_back(std::stod(s));

    if(result.size() != 4)
    {
        throw std::runtime_error("Measurement contain line with incorrect number of data.");
    }

    m_WLData.emplace_back(result[0], result[1], result[2], result[3]);
}

std::ostream & OpticsParser::operator<<(std::ostream & os, const OpticsParser::Parser & parser)
{
    for(const auto & val : parser.m_WLData)
    {
        os << val;
    }
    return os;
}

double OpticsParser::Parser::thickness() const
{
    return m_Thickness;
}

double OpticsParser::Parser::IRTransmittance() const
{
    return m_IRTransmittance;
}

double OpticsParser::Parser::frontEmissivity() const
{
    return m_FrontEmissivity;
}

double OpticsParser::Parser::backEmissivity() const
{
    return m_BackEmissivity;
}

void OpticsParser::Parser::parsePropertyAtTheEnd(const std::string & searchString,
                                                 const std::string & behind,
                                                 const std::string & line,
                                                 double & property)
{
    if(line.find(searchString) != std::string::npos)
    {
        std::string str = line.substr(line.find(behind) + behind.size());
        property = std::stod(str);
    }
}

void OpticsParser::Parser::parseEmissivities(const std::string & line)
{
    if(line.find("Emissivity") != std::string::npos)
    {
        std::string str = line.substr(line.find("Emis=") + 5);
        std::vector<double> result;
        std::istringstream iss(str);
        for(std::string s; iss >> s;)
            result.push_back(std::stod(s));
        if(result.size() != 2)
        {
            throw std::runtime_error("Emissivities line have incorrect number of data.");
        }
        auto front = line.find("front");
        auto back = line.find("back");
        if(front < back)
        {
            m_FrontEmissivity = result[0];
            m_BackEmissivity = result[1];
        }
        else
        {
            m_BackEmissivity = result[0];
            m_FrontEmissivity = result[1];
        }
    }
}

void OpticsParser::Parser::parseNFRCID(const std::string & line)
{
    if(line.find("NFRC") != std::string::npos)
    {
        std::string str = line.substr(line.find("ID:") + 3);
        auto erasePos = str.find('}');
        str.erase(erasePos, 1);
        m_NFRCID = std::stoi(str);
    }
}

void OpticsParser::Parser::parseProductName(const std::string & line)
{
    if(line.find("Product Name") != std::string::npos)
    {
        std::string str = line.substr(line.find("Name:") + 5);
        auto erasePos = str.find('}');
        str.erase(erasePos, 1);
        // Removes all spaces from the beginning of the string
        while(str.size() && isspace(str.front()))
            str.erase(str.begin());
        // Remove all spaces from the end of the string.
        while(str.size() && isspace(str.back()))
            str.pop_back();
        m_ProductName = str;
    }
}

void OpticsParser::Parser::parseProductType(const std::string & line)
{
    if(line.find("Type") != std::string::npos)
    {
        std::string str = line.substr(line.find("Type:") + 5);
        auto erasePos = str.find('}');
        str.erase(erasePos, 1);
        // Removes all spaces from the beginning of the string
        while(str.size() && isspace(str.front()))
            str.erase(str.begin());
        // Remove all spaces from the end of the string.
        while(str.size() && isspace(str.back()))
            str.pop_back();
        m_Type = str;
    }
}

int OpticsParser::Parser::nfrcid() const
{
    return m_NFRCID;
}

double OpticsParser::Parser::conductivity() const
{
    return m_Conductivity;
}

std::vector<OpticsParser::WLData> OpticsParser::Parser::measurements() const
{
    return m_WLData;
}

const std::string & OpticsParser::Parser::productName() const
{
    return m_ProductName;
}

const std::string & OpticsParser::Parser::productType() const
{
    return m_Type;
}


OpticsParser::ProductData OpticsParser::parseFile(std::string const & fname)
{
    OpticsParser::Parser parser(fname);

	OpticsParser::ProductData productData(parser.productName(),
                                          parser.productType(),
                                          parser.nfrcid(),
                                          parser.thickness(),
                                          parser.conductivity(),
                                          parser.IRTransmittance(),
                                          parser.frontEmissivity(),
                                          parser.backEmissivity(),
                                          parser.measurements());

	return productData;
}
