#include <fstream>
#include <sstream>
#include <iostream>

//#include <nlohmann/json.hpp>

#include "Parser.hpp"

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
    parseStringPropertyInsideBraces(line, "Product Name", m_ProductName);
    parseStringPropertyInsideBraces(line, "Type", m_Type);
    parseStringPropertyInsideBraces(line, "Ef_Source", m_FrontEmissivitySource);
    parseStringPropertyInsideBraces(line, "Eb_Source", m_BackEmissivitySource);
    parseStringPropertyInsideBraces(line, "Manufacturer", m_Manufacturer);
    parseStringPropertyInsideBraces(line, "Material", m_Material);
    parseStringPropertyInsideBraces(line, "Coating Name", m_CoatingName);
    parseStringPropertyInsideBraces(line, "Coated Side", m_CoatedSide);
    parseStringPropertyInsideBraces(line, "Substrate Filename", m_SubstrateFilename);
    parseStringPropertyInsideBraces(line, "Appearance", m_Appearance);
    parseStringPropertyInsideBraces(line, "Acceptance", m_Acceptance);
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

#if 0
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
#endif

void OpticsParser::Parser::parseStringPropertyInsideBraces(const std::string & line, std::string search, std::string & property)
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

#if 0
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
#endif

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

std::string OpticsParser::Parser::productName() const
{
    return m_ProductName;
}

std::string OpticsParser::Parser::productType() const
{
    return m_Type;
}

std::string OpticsParser::Parser::frontEmissivitySource() const
{
 return m_FrontEmissivitySource;
}

std::string OpticsParser::Parser::backEmissivitySource() const
{
 return m_BackEmissivitySource;
}

std::string OpticsParser::Parser::manufacturer() const
{
 return m_Manufacturer;
}

std::string OpticsParser::Parser::material() const
{
 return m_Material;
}

std::string OpticsParser::Parser::coatingName() const
{
 return m_CoatingName;
}

std::string OpticsParser::Parser::coatedSide() const
{
 return m_CoatedSide;
}

std::string OpticsParser::Parser::substrateFilename() const
{
 return m_SubstrateFilename;
}

std::string OpticsParser::Parser::appearance() const
{
 return m_Appearance;
}

std::string OpticsParser::Parser::acceptance() const
{
 return m_Acceptance;
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
                                          parser.frontEmissivitySource(),
                                          parser.backEmissivitySource(),
                                          parser.manufacturer(),
                                          parser.material(),
                                          parser.coatingName(),
                                          parser.coatedSide(),
                                          parser.substrateFilename(),
                                          parser.appearance(),
                                          parser.acceptance(),
                                          parser.measurements());

    return productData;
}

#if 0

OpticsParser::ProductData OpticsParser::parseJSONString(std::string const & json_str)
{
    nlohmann::json product_json = nlohmann::json::parse(json_str);

    std::string product_name = product_json.at("name").get<std::string>();
    std::string product_type = product_json.at("type").get<std::string>();
    int nfrc_id = product_json.value("nfrc_id", -1);
    std::string manufacturer = product_json.at("manufacturer").get<std::string>();
    std::string material_name = product_json.at("material_bulk_properties").at("name").get<std::string>();
    std::string coating_name = product_json.at("coating_properties").at("coating_name").get<std::string>();
    std::string coated_side = product_json.at("coating_properties").at("coated_side").get<std::string>();
    std::string substrate_filename = product_json.at("interlayer_properties").at("interlayer_name").get<std::string>();
    std::string appearance = product_json.at("appearance").get<std::string>();
    std::string acceptance = product_json.at("acceptance").get<std::string>();


    nlohmann::json measured_data_json = product_json.at("measured_data");

    double thickness = measured_data_json.at("thickness").get<double>();
    double conductivity = measured_data_json.at("conductivity").get<double>();
    double tir_front = measured_data_json.at("tir_front").get<double>();
    double emissivity_front = measured_data_json.at("emissivity_front").get<double>();
    double emissivity_back = measured_data_json.at("emissivity_back").get<double>();

    nlohmann::json spectral_data_json = product_json.at("spectral_data").at("spectral_data");

    std::vector<OpticsParser::WLData> measurements;

    for(auto & [key, val] : spectral_data_json.items())
    {
        double wl = val.at("wavelength").get<double>();
        double t = val.at("T").get<double>();
        double rf = val.at("Rf").get<double>();
        double rb = val.at("Rb").get<double>();
        measurements.push_back(OpticsParser::WLData(wl, t, rf, rb));
    }


    OpticsParser::ProductData productData(product_name,
                                          product_type,
                                          nfrc_id,
                                          thickness,
                                          conductivity,
                                          tir_front,
                                          emissivity_front,
                                          emissivity_back,
                                          manufacturer,
                                          material_name,
                                          coating_name,
                                          coated_side,
                                          substrate_filename,
                                          appearance,
                                          acceptance,
                                          measurements);

    return productData;
}


OpticsParser::ProductData OpticsParser::parseJSONFile(std::string const & fname)
{
    std::ifstream fin(fname);
    std::string content((std::istreambuf_iterator<char>(fin)), (std::istreambuf_iterator<char>()));
    return parseJSONString(content);
}

#endif
