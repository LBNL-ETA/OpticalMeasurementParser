#include <fstream>
#include <sstream>
#include <iostream>

#include <nlohmann/json.hpp>

#include "Parser.hpp"

OpticsParser::ProductData OpticsParser::Parser::parseFile(const std::string & inputFile)
{
    std::string fileName = inputFile.substr(inputFile.find_last_of("/\\") + 1);
    ProductData product;
    std::ifstream inFile(inputFile);
    std::string line;
    while(std::getline(inFile, line))
    {
        if(line.find('{') != std::string::npos)
        {
            parseHeaderLine(line, product);
        }
        else
        {
            if(!line.empty())
            {
                parseMeasurementLine(line, product);
            }
        }
    }
    product.fileName = fileName;
    // product.token = fileName;

    return product;
}

void OpticsParser::Parser::parseHeaderLine(const std::string & line,
                                           OpticsParser::ProductData & product)
{
    parseUnits(line, product);
    parsePropertyAtTheEnd("Thickness", "}", line, product.thickness);
    parsePropertyAtTheEnd("Conductivity", "}", line, product.conductivity);
    parsePropertyAtTheEnd("IR Transmittance", "TIR=", line, product.IRTransmittance);
    parseEmissivities(line, product);
    parseStringPropertyInsideBraces(line, "Product Name", product.productName);
    parseStringPropertyInsideBraces(line, "Type", product.productType);
    parseStringPropertyInsideBraces(line, "Ef_Source", product.frontEmissivitySource);
    parseStringPropertyInsideBraces(line, "Eb_Source", product.backEmissivitySource);
    parseStringPropertyInsideBraces(line, "Manufacturer", product.manufacturer);
    parseStringPropertyInsideBraces(line, "Material", product.material);
    parseStringPropertyInsideBraces(line, "Coating Name", product.coatingName);
    parseStringPropertyInsideBraces(line, "Coated Side", product.coatedSide);
    parseStringPropertyInsideBraces(line, "Substrate Filename", product.substrateFilename);
    parseStringPropertyInsideBraces(line, "Appearance", product.appearance);
    parseStringPropertyInsideBraces(line, "Acceptance", product.acceptance);
    parseNFRCID(line, product);
}

void OpticsParser::Parser::parseMeasurementLine(const std::string & line, ProductData & product)
{
    std::vector<double> result;
    std::istringstream iss(line);
    for(std::string s; iss >> s;)
        result.push_back(std::stod(s));

    if(result.size() != 4)
    {
        throw std::runtime_error("Measurement contain line with incorrect number of data.");
    }

    product.measurements.emplace_back(result[0], result[1], result[2], result[3]);
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

void OpticsParser::Parser::parseEmissivities(const std::string & line,
                                             OpticsParser::ProductData & product)
{
    if(line.find("Emissivity") != std::string::npos)
    {
        size_t emis_values_idx = line.find("Emis=");
        if(emis_values_idx == std::string::npos)
        {
            // If there is a Emissivity header field but no values that is OK, just return
            return;
        }
        std::string str = line.substr(emis_values_idx + 5);
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
        double ef = -1;
        double eb = -1;
        if(front < back)
        {
            ef = result[0];
            eb = result[1];
        }
        else
        {
            eb = result[0];
            ef = result[1];
        }
        product.frontEmissivity = ef;
        product.backEmissivity = eb;
    }
}

void OpticsParser::Parser::parseUnits(const std::string & line, OpticsParser::ProductData & product)
{
    if(line.find("Units, Wavelength Units") != std::string::npos)
    {
        std::string str = line.substr(line.find("}") + 1);
        std::vector<std::string> result;
        std::istringstream iss(str);
        for(std::string s; iss >> s;)
        {
            result.push_back(s);
        }


        if(result.size() != 2)
        {
            throw std::runtime_error("Units line has incorrect number of values.");
        }

        product.unitSystem = result[0];
        product.wavelengthUnit = result[1];
    }
}

void OpticsParser::Parser::parseNFRCID(const std::string & line,
                                       OpticsParser::ProductData & product)
{
    if(line.find("NFRC") != std::string::npos)
    {
        std::string str = line.substr(line.find("ID:") + 3);
        auto erasePos = str.find('}');
        str.erase(erasePos, 1);
        product.nfrcid = std::stoi(str);
    }
}


void OpticsParser::Parser::parseStringPropertyInsideBraces(const std::string & line,
                                                           std::string search,
                                                           std::string & property)
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

OpticsParser::ProductData parseCheckerToolJson(nlohmann::json const & product_json)
{
    std::string product_name = product_json.at("product_name").get<std::string>();
    std::string product_type = product_json.at("product_type").get<std::string>();
    int nfrc_id = product_json.value("nfrc_id", -1);
    std::string manufacturer = product_json.at("manufacturer").get<std::string>();
    std::string material_name = ""; //TODO
    std::string coating_name =
      product_json.at("coating_properties").at("coating_name").get<std::string>();
    std::string coated_side =
      product_json.at("coating_properties").at("coated_side").get<std::string>();
    std::string substrate_filename = ""; // TODO
    std::string appearance = product_json.at("appearance").get<std::string>();
    std::string acceptance = product_json.at("acceptance").get<std::string>();


    nlohmann::json measured_data_json = product_json.at("measured_data");

    double thickness = measured_data_json.at("thickness").get<double>();
    double conductivity = -1; // TODO
    double tir_front = measured_data_json.at("tir_front").get<double>();
    double emissivity_front = measured_data_json.at("emissivity_front").get<double>();
    double emissivity_back = measured_data_json.at("emissivity_back").get<double>();
    std::string emissivity_front_source = ""; // TODO      
    std::string emissivity_back_source = "";   // TODO
    std::string filename = ""; // TODO
    std::string unit_system = ""; // TODO
    std::string wavelength_units = ""; // TODO


    nlohmann::json spectral_data_json = measured_data_json.at("spectral_data");
    nlohmann::json wavelength_data_json =
      spectral_data_json.at("angle_block")[0].at("wavelength_data");

    std::vector<OpticsParser::WLData> measurements;

    for(auto & [key, val] : wavelength_data_json.items())
    {
        double wl = val.at("w").get<double>();
        double t = val.at("tf").get<double>();
        double rf = val.at("rf").get<double>();
        double rb = val.at("rb").get<double>();
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
                                          emissivity_front_source,
                                          emissivity_back_source,
                                          manufacturer,
                                          material_name,
                                          coating_name,
                                          coated_side,
                                          substrate_filename,
                                          appearance,
                                          acceptance,
                                          filename,
                                          unit_system,
                                          wavelength_units,
                                          measurements);

    return productData;
}

OpticsParser::ProductData parseIGSDBJson(nlohmann::json const & product_json)
{
    std::string product_name = product_json.at("name").get<std::string>();
    std::string product_type = product_json.at("type").get<std::string>();
    int nfrc_id = product_json.value("nfrc_id", -1);
    std::string manufacturer = product_json.at("manufacturer").get<std::string>();
    std::string material_name =
      product_json.at("material_bulk_properties").at("name").get<std::string>();
    std::string coating_name =
      product_json.at("coating_properties").at("coating_name").get<std::string>();
    std::string coated_side =
      product_json.at("coating_properties").at("coated_side").get<std::string>();
    std::string substrate_filename =
      product_json.at("interlayer_properties").at("interlayer_name").get<std::string>();
    std::string appearance = product_json.at("appearance").get<std::string>();
    std::string acceptance = product_json.at("acceptance").get<std::string>();


    nlohmann::json measured_data_json = product_json.at("measured_data");

    double thickness = measured_data_json.at("thickness").get<double>();
    double conductivity = measured_data_json.at("conductivity").get<double>();
    double tir_front = measured_data_json.at("tir_front").get<double>();
    double emissivity_front = measured_data_json.at("emissivity_front").get<double>();
    double emissivity_back = measured_data_json.at("emissivity_back").get<double>();
    std::string emissivity_front_source =
      measured_data_json.at("emissivity_front_source").get<std::string>();
    std::string emissivity_back_source =
      measured_data_json.at("emissivity_back_source").get<std::string>();
    std::string filename = ""; // TODO
    std::string unit_system = ""; // TODO
    std::string wavelength_units = ""; // TODO


    nlohmann::json spectral_data_json = measured_data_json.at("spectral_data");
    nlohmann::json wavelength_data_json = spectral_data_json.at("spectral_data");

    std::vector<OpticsParser::WLData> measurements;

    for(auto & [key, val] : wavelength_data_json.items())
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
                                          emissivity_front_source,
                                          emissivity_back_source,
                                          manufacturer,
                                          material_name,
                                          coating_name,
                                          coated_side,
                                          substrate_filename,
                                          appearance,
                                          acceptance,
                                          filename,
                                          unit_system,
                                          wavelength_units,
                                          measurements);

    return productData;
}

OpticsParser::ProductData OpticsParser::Parser::parseJSONString(std::string const & json_str)
{
    nlohmann::json product_json = nlohmann::json::parse(json_str);

    // There are now two different json formats, one output from the IGSDB and one
    // that is input to the checker tool app.  They are similar but different enough that
    // they are getting two different parsing paths with the common aspects abstracted out
    if(product_json.find("name") != product_json.end())
    {
        // The IGSDB json format has a "name" field
        return parseIGSDBJson(product_json);
    }
    else
    {
        // The checker tool JSON format has a "product_name" field instead of a "name" field
        if(product_json.find("product_name") == product_json.end())
        {
            throw std::runtime_error("Unable to parse json.  It does not appear to be "
                                     "either of the two recognized formats.  Currently "
                                     "only IGSDB and checker tool formats are supported.");
        }
        return parseCheckerToolJson(product_json);
    }
}


OpticsParser::ProductData OpticsParser::Parser::parseJSONFile(std::string const & fname)
{
    std::ifstream fin(fname);
    std::string content((std::istreambuf_iterator<char>(fin)), (std::istreambuf_iterator<char>()));
    return parseJSONString(content);
}
