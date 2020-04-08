#include <fstream>
#include <sstream>
#include <iostream>

#include <nlohmann/json.hpp>

#include "Parser.hpp"
std::shared_ptr<OpticsParser::ProductData>
  OpticsParser::Parser::parseFile(const std::string & inputFile)
{
    std::string fileName = inputFile.substr(inputFile.find_last_of("/\\") + 1);
    std::shared_ptr<OpticsParser::ProductData> product(new OpticsParser::ProductData);
    product->measurements = std::vector<OpticsParser::WLData>();
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
    product->fileName = fileName;
    // product->token = fileName;

    return product;
}

void OpticsParser::Parser::parseHeaderLine(const std::string & line,
                                           std::shared_ptr<OpticsParser::ProductData> product)
{
    parseUnits(line, product);
    parsePropertyAtTheEnd("Thickness", "}", line, product->thickness);
    parsePropertyAtTheEnd("Conductivity", "}", line, product->conductivity);
    parsePropertyAtTheEnd("IR Transmittance", "TIR=", line, product->IRTransmittance);
    parseEmissivities(line, product);
    parseStringPropertyInsideBraces(line, "Product Name", product->productName);
    parseStringPropertyInsideBraces(line, "Type", product->productType);
    parseStringPropertyInsideBraces(line, "Ef_Source", product->frontEmissivitySource);
    parseStringPropertyInsideBraces(line, "Eb_Source", product->backEmissivitySource);
    parseStringPropertyInsideBraces(line, "Manufacturer", product->manufacturer);
    parseStringPropertyInsideBraces(line, "Material", product->material);
    parseStringPropertyInsideBraces(line, "Coating Name", product->coatingName);
    parseStringPropertyInsideBraces(line, "Coated Side", product->coatedSide);
    parseStringPropertyInsideBraces(line, "Substrate Filename", product->substrateFilename);
    parseStringPropertyInsideBraces(line, "Appearance", product->appearance);
    parseStringPropertyInsideBraces(line, "Acceptance", product->acceptance);
    parseStringPropertyInsideBraces(line, "Extrapolation", product->extrapolation);
    parseBoolPropertyInsideBraces(line, "Specularity", product->specularity);
    parseDoublePropertyInsideBraces(line, "Permeability Factor", product->permeabilityFactor);
    parseNFRCID(line, product);
    parseAERCID(line, product);
}

OpticsParser::WLData parseDirectMeasurementLine(std::vector<double> const & values)
{
    double wl = values[0];
    double tf = values[1];
    double tb = tf;
    double rf = values[2];
    double rb = values[3];
    OpticsParser::MeasurementComponent directValues{tf, tb, rf, rb};
    return OpticsParser::WLData(wl, directValues);
}

OpticsParser::WLData parseDiffuseMeasurementLine(std::vector<double> const & values)
{
    double wl = values[0];
    double tfDirect = values[1];
    double tbDirect = values[3];
    double rfDirect = values[5];
    double rbDirect = values[7];
    double tfDiffuse = values[2];
    double tbDiffuse = values[4];
    double rfDiffuse = values[6];
    double rbDiffuse = values[8];

    OpticsParser::MeasurementComponent directValues{tfDirect, tbDirect, rfDirect, rbDirect};
    OpticsParser::MeasurementComponent diffuseValues{tfDiffuse, tbDiffuse, rfDiffuse, rbDiffuse};
    return OpticsParser::WLData(wl, directValues, diffuseValues);
}


void OpticsParser::Parser::parseMeasurementLine(const std::string & line,
                                                std::shared_ptr<OpticsParser::ProductData> product)
{
    std::vector<double> result;
    std::istringstream iss(line);
    for(std::string s; iss >> s;)
        result.push_back(std::stod(s));

    std::map<size_t, std::function<OpticsParser::WLData(std::vector<double> const &)>>
      measuredValueToParser;
    measuredValueToParser[4] = &parseDirectMeasurementLine;
    measuredValueToParser[9] = &parseDiffuseMeasurementLine;


    auto parser = measuredValueToParser.find(result.size());

    std::vector<OpticsParser::WLData> measurements;

    if(parser != measuredValueToParser.end())
    {
        auto parsedValues = parser->second(result);
        product->measurements.value().push_back(parsedValues);
    }
    else
    {
        throw std::runtime_error("Unknown measured data line format.");
    }
}


void OpticsParser::Parser::parseEmissivities(const std::string & line,
                                             std::shared_ptr<OpticsParser::ProductData> product)
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
        product->frontEmissivity = ef;
        product->backEmissivity = eb;
    }
}

void OpticsParser::Parser::parseUnits(const std::string & line,
                                      std::shared_ptr<OpticsParser::ProductData> product)
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

        product->unitSystem = result[0];
        product->wavelengthUnit = result[1];
    }
}

void OpticsParser::Parser::parseNFRCID(const std::string & line,
                                       std::shared_ptr<OpticsParser::ProductData> product)
{
    if(line.find("NFRC") != std::string::npos)
    {
        std::string str = line.substr(line.find("ID:") + 3);
        auto erasePos = str.find('}');
        str.erase(erasePos, 1);
        product->nfrcid = std::stoi(str);
    }
}

void OpticsParser::Parser::parseAERCID(const std::string & line,
                                       std::shared_ptr<OpticsParser::ProductData> product)
{
    if(line.find("AERC") != std::string::npos)
    {
        std::string str = line.substr(line.find("ID:") + 3);
        auto erasePos = str.find('}');
        str.erase(erasePos, 1);
        product->aercID = std::stoi(str);
    }
}

void OpticsParser::Parser::parseBoolPropertyInsideBraces(const std::string & line,
                                                         std::string search,
                                                         std::optional<bool> & property)
{
    std::string val("");
    parseStringPropertyInsideBraces(line, search, val);
    if(val.length() > 1)
    {
        std::string upperCase = val;
#pragma warning(push)
#pragma warning(disable : 4244)
        std::for_each(upperCase.begin(), upperCase.end(), [](char & c) { c = ::toupper(c); });
#pragma warning(pop)
        if(upperCase == "TRUE")
        {
            property = true;
        }
        else if(upperCase == "FALSE")
        {
            property = false;
        }
        else
        {
            std::stringstream msg;
            msg << "Unable to convert " << val << " to a boolean when parsing field: " << search;
            throw std::runtime_error(msg.str());
        }
    }
}

void OpticsParser::Parser::parseDoublePropertyInsideBraces(const std::string & line,
                                                           std::string search,
                                                           std::optional<double> & property)
{
    std::string val("");
    parseStringPropertyInsideBraces(line, search, val);
    if(val.length() > 0)
    {
        property = std::stod(val);
    }
}

template<typename T>
std::optional<T> get_optional_field(nlohmann::json const & json, std::string const & field_name)
{
    std::optional<T> data;
    if(json.count(field_name))
    {
        auto field = json.at(field_name);
        if(!field.is_null())
        {
            data = json.at(field_name).get<T>();
        }
    }
    return data;
}

#if 0
OpticsParser::ProductData parseCheckerToolJson_OLD(nlohmann::json const & product_json)
{
    std::string product_name = product_json.at("product_name").get<std::string>();
    std::string product_type = product_json.at("product_type").get<std::string>();
    int nfrc_id = product_json.value("nfrc_id", -1);
    std::string manufacturer = product_json.at("manufacturer").get<std::string>();
    std::string material_name = "";   // TODO
    std::string coating_name =
      product_json.at("coating_properties").at("coating_name").get<std::string>();
    std::string coated_side =
      product_json.at("coating_properties").at("coated_side").get<std::string>();
    std::string substrate_filename = "";   // TODO
    std::string appearance = product_json.at("appearance").get<std::string>();
    std::string acceptance = product_json.at("acceptance").get<std::string>();


    nlohmann::json measured_data_json = product_json.at("measured_data");

    double thickness = measured_data_json.at("thickness").get<double>();
    double conductivity = -1;   // TODO
    double tir_front = measured_data_json.at("tir_front").get<double>();
    double emissivity_front = measured_data_json.at("emissivity_front").get<double>();
    double emissivity_back = measured_data_json.at("emissivity_back").get<double>();
    std::string emissivity_front_source = "";   // TODO
    std::string emissivity_back_source = "";    // TODO
    std::string filename = "";                  // TODO
    std::string unit_system = "";               // TODO
    std::string wavelength_units = "";          // TODO


    nlohmann::json spectral_data_json = measured_data_json.at("spectral_data");
    nlohmann::json wavelength_data_json =
      spectral_data_json.at("angle_block")[0].at("wavelength_data");

    std::vector<OpticsParser::WLData> measurements;

    for(nlohmann::json::iterator itr = wavelength_data_json.begin();
        itr != wavelength_data_json.end();
        ++itr)
    {
        auto val = itr.value();
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
#endif

std::shared_ptr<OpticsParser::ProductData> parseCheckerToolJson(nlohmann::json const & product_json)
{
    std::shared_ptr<OpticsParser::ProductData> product(new OpticsParser::ProductData);
    product->productName = product_json.at("product_name").get<std::string>();
    product->productType = product_json.at("product_type").get<std::string>();

    product->nfrcid = get_optional_field<int>(product_json, "nfrc_id");
    product->manufacturer = product_json.at("manufacturer").get<std::string>();
    if(product_json.count("material_bulk_properties"))
    {
        product->material =
          get_optional_field<std::string>(product_json.at("material_bulk_properties"), "name");
    }

    if(product_json.count("coating_properties"))
    {
        product->coatingName =
          get_optional_field<std::string>(product_json.at("coating_properties"), "coating_name");
        product->coatedSide =
          get_optional_field<std::string>(product_json.at("coating_properties"), "coated_side");
    }

    if(product_json.count("interlayer_properties"))
    {
        product->substrateFilename = get_optional_field<std::string>(
          product_json.at("interlayer_properties"), "interlayer_name");
    }


    product->appearance = get_optional_field<std::string>(product_json, "appearance");
    product->acceptance = get_optional_field<std::string>(product_json, "acceptance");
    product->fileName = get_optional_field<std::string>(product_json, "filename");
    product->unitSystem = get_optional_field<std::string>(product_json, "unit_system");

    nlohmann::json measured_data_json = product_json.at("measured_data");

    product->thickness = get_optional_field<double>(
      measured_data_json, "thickness");   // measured_data_json.at("thickness").get<double>();
    if(measured_data_json.count("bulk_properties_override"))
    {
        product->conductivity = get_optional_field<double>(
          measured_data_json.at("bulk_properties_override"), "thermal_conductivity");
    }

    product->IRTransmittance = get_optional_field<double>(measured_data_json, "tir_front");
    product->frontEmissivity = get_optional_field<double>(measured_data_json, "emissivity_front");
    product->backEmissivity = get_optional_field<double>(measured_data_json, "emissivity_back");

    product->frontEmissivitySource =
      get_optional_field<std::string>(measured_data_json, "emissivity_front_source");

    product->backEmissivitySource =
      get_optional_field<std::string>(measured_data_json, "emissivity_back_source");

    product->backEmissivitySource =
      get_optional_field<std::string>(measured_data_json, "wavelength_units");

    std::vector<OpticsParser::WLData> measurements;

    nlohmann::json spectral_data_json = measured_data_json.at("spectral_data");
    if(!spectral_data_json.is_null())
    {
        nlohmann::json wavelength_data_json =
          spectral_data_json.at("angle_block")[0].at("wavelength_data");

        for(nlohmann::json::iterator itr = wavelength_data_json.begin();
            itr != wavelength_data_json.end();
            ++itr)
        {
            auto val = itr.value();
            double wl = val.at("w").get<double>();
            double t = val.at("tf").get<double>();
            double rf = val.at("rf").get<double>();
            double rb = val.at("rb").get<double>();
            OpticsParser::MeasurementComponent directValues{t, t, rf, rb};
            measurements.push_back(OpticsParser::WLData(wl, directValues));
        }
    }
    if(!measurements.empty())
    {
        product->measurements = measurements;
    }

    return product;
}


std::shared_ptr<OpticsParser::ProductData>
  parseIGSDBJsonUncomposedProduct(nlohmann::json const & product_json)
{
    std::shared_ptr<OpticsParser::ProductData> product(new OpticsParser::ProductData);
    product->productName = product_json.at("name").get<std::string>();
    product->productType = product_json.at("type").get<std::string>();
    product->subtype = product_json.at("subtype").get<std::string>();

    product->nfrcid = get_optional_field<int>(product_json, "nfrc_id");
    product->manufacturer = product_json.at("manufacturer_name").get<std::string>();
    product->material = get_optional_field<std::string>(product_json, "material_bulk_properties");

    if(product_json.count("coating_properties"))
    {
        product->coatingName =
          get_optional_field<std::string>(product_json.at("coating_properties"), "coating_name");
        product->coatedSide =
          get_optional_field<std::string>(product_json.at("coating_properties"), "coated_side");
    }

    if(product_json.count("interlayer_properties"))
    {
        product->substrateFilename = get_optional_field<std::string>(
          product_json.at("interlayer_properties"), "interlayer_name");
    }


    product->appearance = get_optional_field<std::string>(product_json, "appearance");
    product->acceptance = get_optional_field<std::string>(product_json, "acceptance");
    product->fileName = get_optional_field<std::string>(product_json, "filename");
    product->unitSystem = get_optional_field<std::string>(product_json, "unit_system");

    nlohmann::json measured_data_json = product_json.at("measured_data");

    product->thickness = get_optional_field<double>(measured_data_json, "thickness");
    product->conductivity = get_optional_field<double>(measured_data_json, "conductivity");
    product->IRTransmittance = get_optional_field<double>(measured_data_json, "tir_front");
    product->frontEmissivity = get_optional_field<double>(measured_data_json, "emissivity_front");
    product->backEmissivity = get_optional_field<double>(measured_data_json, "emissivity_back");

    product->frontEmissivitySource =
      get_optional_field<std::string>(measured_data_json, "emissivity_front_source");

    product->backEmissivitySource =
      get_optional_field<std::string>(measured_data_json, "emissivity_back_source");

    product->backEmissivitySource =
      get_optional_field<std::string>(measured_data_json, "wavelength_units");


    nlohmann::json spectral_data_json = product_json.at("spectral_data");
    if(!spectral_data_json.is_null())
    {
        nlohmann::json wavelength_data_json = spectral_data_json.at("spectral_data");

        std::vector<OpticsParser::WLData> measurements;

        for(nlohmann::json::iterator itr = wavelength_data_json.begin();
            itr != wavelength_data_json.end();
            ++itr)
        {
            auto val = itr.value();
            double wl = val.at("wavelength").get<double>();
            double t = val.at("T").get<double>();
            double rf = val.at("Rf").get<double>();
            double rb = val.at("Rb").get<double>();
            OpticsParser::MeasurementComponent directValues{t, t, rf, rb};
            measurements.push_back(OpticsParser::WLData(wl, directValues));
        }
        if(!measurements.empty())
        {
            product->measurements = measurements;
        }
    }
    return product;
}

std::shared_ptr<OpticsParser::ProductGeometry>
  parseVenetianGeometry(nlohmann::json const & geometry_json)
{
    auto slatWidth = geometry_json.at("slat_width").get<double>();
    auto slatSpacing = geometry_json.at("slat_spacing").get<double>();
    auto slatCurvature = geometry_json.at("slat_curvature").get<double>();
    auto numberSegments = geometry_json.at("number_segments").get<int>();

    return std::shared_ptr<OpticsParser::ProductGeometry>(
      new OpticsParser::VenetianGeometry(slatWidth, slatSpacing, slatCurvature, numberSegments));
}

std::shared_ptr<OpticsParser::ProductGeometry>
  parseWovenGeometry(nlohmann::json const & geometry_json)
{
    auto threadDiameter = geometry_json.at("thread_diameter").get<double>();
    auto threadSpacing = geometry_json.at("thread_spacing").get<double>();
    auto shadeThickness = geometry_json.at("shade_thickness").get<double>();

    return std::shared_ptr<OpticsParser::ProductGeometry>(
      new OpticsParser::WovenGeometry(threadDiameter, threadSpacing, shadeThickness));
}

std::shared_ptr<OpticsParser::ProductGeometry> parseGeometry(std::string const & subtype,
                                                             nlohmann::json const & geometry_json)
{
    std::map<std::string,
             std::function<std::shared_ptr<OpticsParser::ProductGeometry>(nlohmann::json const &)>>
      mapping;

    mapping["venetian"] = &parseVenetianGeometry;
    mapping["woven"] = &parseWovenGeometry;

    auto itr = mapping.find(subtype);
    if(itr != mapping.end())
    {
        return itr->second(geometry_json);
    }
    else
    {
        std::stringstream msg;
        msg << "Subtype " << subtype << " geometry not yet supported.";
        throw std::runtime_error(msg.str());
    }
}

std::shared_ptr<OpticsParser::ProductData>
  parseIGSDBJsonComposedProduct(nlohmann::json const & product_json)
{
    auto subtype = product_json.at("subtype").get<std::string>();
    auto composition_information = product_json.at("composition_information");
    auto product_material = composition_information.at("materials")[0];
    auto product_geometry = composition_information.at("geometry");
    auto material = parseIGSDBJsonUncomposedProduct(product_material);
    auto geometry = parseGeometry(subtype, product_geometry);
    std::shared_ptr<OpticsParser::CompositionInformation> compositionInformation(
      new OpticsParser::CompositionInformation{material, geometry});
    auto product = parseIGSDBJsonUncomposedProduct(product_json);
    std::shared_ptr<OpticsParser::ProductData> composedProduct(
      new OpticsParser::ComposedProductData(*product, compositionInformation));
    return composedProduct;
}

std::shared_ptr<OpticsParser::ProductData> parseIGSDBJson(nlohmann::json const & product_json)
{
    if(product_json.count("composition_information"))
    {
        return parseIGSDBJsonComposedProduct(product_json);
    }
    else
    {
        return parseIGSDBJsonUncomposedProduct(product_json);
    }
}

std::shared_ptr<OpticsParser::ProductData>
  OpticsParser::Parser::parseJSONString(std::string const & json_str)
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


std::shared_ptr<OpticsParser::ProductData>
  OpticsParser::Parser::parseJSONFile(std::string const & fname)
{
    std::ifstream fin(fname);
    std::string content((std::istreambuf_iterator<char>(fin)), (std::istreambuf_iterator<char>()));
    return parseJSONString(content);
}

std::shared_ptr<OpticsParser::ProductData> OpticsParser::parseFile(const std::string & inputFile)
{
    OpticsParser::Parser parser;
    return parser.parseFile(inputFile);
}

std::shared_ptr<OpticsParser::ProductData> OpticsParser::parseJSONString(std::string const & json)
{
    OpticsParser::Parser parser;
    return parser.parseJSONString(json);
}

std::shared_ptr<OpticsParser::ProductData> OpticsParser::parseJSONFile(std::string const & fname)
{
    OpticsParser::Parser parser;
    return parser.parseJSONFile(fname);
}
