#include <fstream>
#include <sstream>
#include <iostream>
#include <cctype>
#include <regex>
#include <algorithm>
#include <nlohmann/json.hpp>
#include <xmlParser/xmlParser.h>
#include "Parser.hpp"

namespace OpticsParser
{
    std::string toLower(std::string s)
    {
#ifdef _MSC_VER
#    pragma warning(push)
#    pragma warning(disable : 4244)
#endif
        std::transform(s.begin(), s.end(), s.begin(), ::tolower);
#ifdef _MSC_VER
#    pragma warning(pop)
#endif
        return s;
    }

    std::shared_ptr<ProductData> Parser::parseFile(const std::string & inputFile)
    {
        std::string fileName = inputFile.substr(inputFile.find_last_of("/\\") + 1);
        std::shared_ptr<ProductData> product(new ProductData);
        product->measurements = std::vector<WLData>();
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

    std::optional<double> parseOptionalDoubleNode(XMLParser::XMLNode const & node)
    {
        std::optional<double> result;
        auto txt = node.getText();
        if(txt)
        {
            result = std::stod(txt);
        }
        return result;
    }


    void Parser::parseHeaderLine(const std::string & line, std::shared_ptr<ProductData> product)
    {
        parseUnits(line, product);
        parsePropertyAtTheEnd("Thickness", "}", line, product->thickness);
        parsePropertyAtTheEnd("Conductivity", "}", line, product->conductivity);
        parsePropertyAtTheEnd("IR Transmittance", "TIR=", line, product->IRTransmittance);
        parseEmissivities(line, product);
        parseStringPropertyInsideBraces(line, "Product Name", product->productName);
        product->productType = "glazing";   // There are only glazing optics files.
        parseStringPropertyInsideBraces(line, "Type", product->productSubtype);
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

    WLData parseDirectMeasurementLine(std::vector<double> const & values)
    {
        double wl = values[0];
        double tf = values[1];
        double tb = tf;
        double rf = values[2];
        double rb = values[3];
        MeasurementComponent directValues{tf, tb, rf, rb};
        return WLData(wl, directValues);
    }

    WLData parseDiffuseMeasurementLine(std::vector<double> const & values)
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

        MeasurementComponent directValues{tfDirect, tbDirect, rfDirect, rbDirect};
        MeasurementComponent diffuseValues{tfDiffuse, tbDiffuse, rfDiffuse, rbDiffuse};
        return WLData(wl, directValues, diffuseValues);
    }

    void Parser::parseMeasurementLine(const std::string & line,
                                      std::shared_ptr<ProductData> product)
    {
        std::vector<double> result;
        std::istringstream iss(line);
        for(std::string s; iss >> s;)
            result.push_back(std::stod(s));

        std::map<size_t, std::function<WLData(std::vector<double> const &)>> measuredValueToParser;
        measuredValueToParser[4] = &parseDirectMeasurementLine;
        measuredValueToParser[9] = &parseDiffuseMeasurementLine;


        auto parser = measuredValueToParser.find(result.size());


        if(parser != measuredValueToParser.end())
        {
            std::vector<WLData> & measurements =
              std::get<std::vector<WLData>>(product->measurements.value());
            auto parsedValues = parser->second(result);
            measurements.push_back(parsedValues);
        }
        else
        {
            throw std::runtime_error("Unknown measured data line format.");
        }
    }


    void Parser::parseEmissivities(const std::string & line, std::shared_ptr<ProductData> product)
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

    void Parser::parseUnits(const std::string & line, std::shared_ptr<ProductData> product)
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

    void Parser::parseNFRCID(const std::string & line, std::shared_ptr<ProductData> product)
    {
        if(line.find("NFRC") != std::string::npos)
        {
            std::string str = line.substr(line.find("ID:") + 3);
            auto erasePos = str.find('}');
            str.erase(erasePos, 1);
            product->nfrcid = std::stoi(str);
        }
    }

    void Parser::parseAERCID(const std::string & line, std::shared_ptr<ProductData> product)
    {
        if(line.find("AERC") != std::string::npos)
        {
            std::string str = line.substr(line.find("ID:") + 3);
            auto erasePos = str.find('}');
            str.erase(erasePos, 1);
            product->aercID = std::stoi(str);
        }
    }

    void Parser::parseBoolPropertyInsideBraces(const std::string & line,
                                               std::string search,
                                               std::optional<bool> & property)
    {
        std::string val("");
        parseStringPropertyInsideBraces(line, search, val);
        if(val.length() > 1)
        {
            std::string upperCase = val;
#ifdef _MSC_VER
#    pragma warning(push)
#    pragma warning(disable : 4244)
#endif
            std::for_each(upperCase.begin(), upperCase.end(), [](char & c) { c = ::toupper(c); });
#ifdef _MSC_VER
#    pragma warning(pop)
#endif
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
                msg << "Unable to convert " << val
                    << " to a boolean when parsing field: " << search;
                throw std::runtime_error(msg.str());
            }
        }
    }

    void Parser::parseDoublePropertyInsideBraces(const std::string & line,
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


    std::shared_ptr<ProductData> parseCheckerToolJson(nlohmann::json const & product_json)
    {
        std::shared_ptr<ProductData> product(new ProductData);
        product->productName = product_json.at("product_name").get<std::string>();
        product->productType = product_json.at("product_type").get<std::string>();

        product->nfrcid = get_optional_field<int>(product_json, "nfrc_id");
        product->cgdbShadingLayerId =
          get_optional_field<int>(product_json, "cgdb_shading_layer_id");
        product->cgdbShadeMaterialId =
          get_optional_field<int>(product_json, "cgdb_shade_material_id");
        product->igdbDatabaseVersion =
          get_optional_field<std::string>(product_json, "igdb_database_version");
        product->cgdbDatabaseVersion =
          get_optional_field<std::string>(product_json, "cgdb_database_version");
        product->manufacturer = product_json.at("manufacturer").get<std::string>();
        if(product_json.count("material_bulk_properties"))
        {
            product->material =
              get_optional_field<std::string>(product_json.at("material_bulk_properties"), "name");
        }

        if(product_json.count("coating_properties"))
        {
            product->coatingName = get_optional_field<std::string>(
              product_json.at("coating_properties"), "coating_name");
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
        product->frontEmissivity =
          get_optional_field<double>(measured_data_json, "emissivity_front");
        product->backEmissivity = get_optional_field<double>(measured_data_json, "emissivity_back");

        product->frontEmissivitySource =
          get_optional_field<std::string>(measured_data_json, "emissivity_front_source");

        product->backEmissivitySource =
          get_optional_field<std::string>(measured_data_json, "emissivity_back_source");

        product->backEmissivitySource =
          get_optional_field<std::string>(measured_data_json, "wavelength_units");

        std::vector<WLData> measurements;

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
                MeasurementComponent directValues{t, t, rf, rb};
                measurements.push_back(WLData(wl, directValues));
            }
        }
        if(!measurements.empty())
        {
            product->measurements = measurements;
        }

        return product;
    }

    void parseDualBandValues(std::shared_ptr<ProductData> product,
                             nlohmann::json const & spectral_data_json)
    {
        if(spectral_data_json.count("dual_band_values") == 0
           || spectral_data_json.at("dual_band_values").empty())
        {
            return;
        }
        auto dual_band = spectral_data_json.at("dual_band_values");
        DualBandValues specular;
        DualBandValues diffuse;

        specular.solarTransmittanceFront = get_optional_field<double>(dual_band, "Tf_sol_specular");
        specular.solarReflectanceFront = get_optional_field<double>(dual_band, "Rf_sol_specular");
        specular.solarReflectanceBack = get_optional_field<double>(dual_band, "Rb_sol_specular");
        specular.solarTransmittanceBack = get_optional_field<double>(dual_band, "Tb_sol_specular");

        specular.visibleTransmittanceFront =
          get_optional_field<double>(dual_band, "Tf_vis_specular");
        specular.visibleTransmittanceBack =
          get_optional_field<double>(dual_band, "Tb_vis_specular");
        specular.visibleReflectanceFront = get_optional_field<double>(dual_band, "Rf_vis_specular");
        specular.visibleReflectanceBack = get_optional_field<double>(dual_band, "Rb_vis_specular");

        diffuse.solarTransmittanceFront = get_optional_field<double>(dual_band, "Tf_sol_diffuse");
        diffuse.solarReflectanceFront = get_optional_field<double>(dual_band, "Rf_sol_diffuse");
        diffuse.solarReflectanceBack = get_optional_field<double>(dual_band, "Rb_sol_diffuse");
        diffuse.solarTransmittanceBack = get_optional_field<double>(dual_band, "Tb_sol_diffuse");

        diffuse.visibleTransmittanceFront = get_optional_field<double>(dual_band, "Tf_vis_diffuse");
        diffuse.visibleTransmittanceBack = get_optional_field<double>(dual_band, "Tb_vis_diffuse");
        diffuse.visibleReflectanceFront = get_optional_field<double>(dual_band, "Rf_vis_diffuse");
        diffuse.visibleReflectanceBack = get_optional_field<double>(dual_band, "Rb_vis_diffuse");

        product->dualBandSpecular = specular;
        product->dualBandDiffuse = diffuse;
    }

    void parseIntegratedResults(std::shared_ptr<ProductData> product,
                                nlohmann::json const & product_json)
    {
        if(product_json.count("integrated_results_summary") == 0
           || product_json.at("integrated_results_summary").empty())
        {
            return;
        }
        auto integrated_results = product_json.at("integrated_results_summary")[0];
        PrecalculatedResults result;

        result.solarTransmittanceFront = get_optional_field<double>(integrated_results, "tfsol");
        result.solarReflectanceFront = get_optional_field<double>(integrated_results, "rfsol");
        result.visibleTransmittanceFront = get_optional_field<double>(integrated_results, "tfvis");
        result.visibleReflectanceFront = get_optional_field<double>(integrated_results, "rfvis");
        result.visibleReflectanceBack = get_optional_field<double>(integrated_results, "rbvis");
        result.dwTransmittance = get_optional_field<double>(integrated_results, "tdw");
        result.uvTransmittance = get_optional_field<double>(integrated_results, "tuv");
        result.spfTransmittance = get_optional_field<double>(integrated_results, "tspf");
        auto tcie_x = get_optional_field<double>(integrated_results, "tciex");
        auto tcie_y = get_optional_field<double>(integrated_results, "tciey");
        auto tcie_z = get_optional_field<double>(integrated_results, "tciez");
        if(tcie_x.has_value() && tcie_y.has_value() && tcie_z.has_value())
        {
            result.cieTransmittance = CIEValue{*tcie_x, *tcie_y, *tcie_z};
        }
        auto rfcie_x = get_optional_field<double>(integrated_results, "rfciex");
        auto rfcie_y = get_optional_field<double>(integrated_results, "rfciey");
        auto rfcie_z = get_optional_field<double>(integrated_results, "rfciez");
        if(rfcie_x.has_value() && rfcie_y.has_value() && rfcie_z.has_value())
        {
            result.cieReflectanceFront = CIEValue{*rfcie_x, *rfcie_y, *rfcie_z};
        }
        product->precalculatedResults = result;
    }

    std::shared_ptr<ProductData>
      parseIGSDBJsonUncomposedProduct_v1(nlohmann::json const & product_json)
    {
        std::shared_ptr<ProductData> product(new ProductData);
        product->name = product_json.at("name").get<std::string>();
        product->productName = get_optional_field<std::string>(product_json, "product_name");
        product->productType = product_json.at("type").get<std::string>();
        product->productSubtype = get_optional_field<std::string>(product_json, "subtype");

        product->nfrcid = get_optional_field<int>(product_json, "nfrc_id");
        product->cgdbShadingLayerId =
          get_optional_field<int>(product_json, "cgdb_shading_layer_id");
        product->cgdbShadeMaterialId =
          get_optional_field<int>(product_json, "cgdb_shade_material_id");
        product->igdbDatabaseVersion =
          get_optional_field<std::string>(product_json, "igdb_database_version");
        product->cgdbDatabaseVersion =
          get_optional_field<std::string>(product_json, "cgdb_database_version");
        product->igdbChecksum = get_optional_field<int>(product_json, "igdb_checksum");
        product->cgdbChecksum = get_optional_field<int>(product_json, "cgdb_checksum");
        product->manufacturer = product_json.at("manufacturer_name").get<std::string>();
        product->material =
          get_optional_field<std::string>(product_json, "material_bulk_properties");

        product->coatingName = get_optional_field<std::string>(product_json, "coating_name");
        product->coatedSide = get_optional_field<std::string>(product_json, "coated_side");

        product->substrateFilename =
          get_optional_field<std::string>(product_json, "interlayer_name");


        product->appearance = get_optional_field<std::string>(product_json, "appearance");
        product->acceptance = get_optional_field<std::string>(product_json, "acceptance");
        product->fileName = get_optional_field<std::string>(product_json, "filename");
        product->dataFileName = get_optional_field<std::string>(product_json, "data_file_name");
        product->unitSystem = get_optional_field<std::string>(product_json, "unit_system");
        product->opticalOpenness = get_optional_field<double>(product_json, "optical_openness");

        nlohmann::json measured_data_json = product_json.at("measured_data");

        product->thickness = get_optional_field<double>(measured_data_json, "thickness");
        product->conductivity = get_optional_field<double>(measured_data_json, "conductivity");
        product->IRTransmittance = get_optional_field<double>(measured_data_json, "tir_front");
        product->frontEmissivity =
          get_optional_field<double>(measured_data_json, "emissivity_front");
        product->backEmissivity = get_optional_field<double>(measured_data_json, "emissivity_back");

        product->frontEmissivitySource =
          get_optional_field<std::string>(measured_data_json, "emissivity_front_source");

        product->backEmissivitySource =
          get_optional_field<std::string>(measured_data_json, "emissivity_back_source");

        product->permeabilityFactor =
          get_optional_field<double>(measured_data_json, "permeability_factor");

        product->backEmissivitySource =
          get_optional_field<std::string>(measured_data_json, "wavelength_units");


        nlohmann::json spectral_data_json = product_json.at("spectral_data");
        if(!spectral_data_json.is_null())
        {
            nlohmann::json wavelength_data_json = spectral_data_json.at("spectral_data");

            std::vector<WLData> measurements;

            for(nlohmann::json::iterator itr = wavelength_data_json.begin();
                itr != wavelength_data_json.end();
                ++itr)
            {
                auto val = itr.value();
                double wl = val.at("wavelength").get<double>();
                double t = val.at("T").get<double>();
                double rf = val.at("Rf").get<double>();
                double rb = val.at("Rb").get<double>();
                MeasurementComponent directValues{t, t, rf, rb};
                measurements.push_back(WLData(wl, directValues));
            }
            if(!measurements.empty())
            {
                product->measurements = measurements;
            }
            parseDualBandValues(product, spectral_data_json);
        }
        parseIntegratedResults(product, product_json);

        return product;
    }

    PVPowerProperties parsePowerProperties(nlohmann::json const & power_properties_json)
    {
        PVPowerProperties res;
        for(auto & tempeartaure_power_properties_json : power_properties_json)
        {
            double temperature =
              std::stod(tempeartaure_power_properties_json.at("temperature").get<std::string>());
            auto values_json = tempeartaure_power_properties_json.at("values");
            std::vector<PVPowerProperty> properties;
            for(auto & value_json : values_json)
            {
                double jsc = std::stod(value_json.at("jsc").get<std::string>());
                double voc = std::stod(value_json.at("voc").get<std::string>());
                double ff = std::stod(value_json.at("ff").get<std::string>());
                properties.push_back(PVPowerProperty{jsc, voc, ff});
            }
            res[temperature] = properties;
        }
        return res;
    }

    std::shared_ptr<ProductData>
      parseIGSDBJsonUncomposedProduct_v2(nlohmann::json const & product_json)
    {
        /*
        NOTE:  All values in v2 json are strings.  Actually this is unfortunately not true.
		Some values are strings like wavelength values but some are numbers like thickness.
        */
        std::shared_ptr<ProductData> product = std::make_shared<ProductData>();
        product->name = product_json.at("name").get<std::string>();
        product->productName = get_optional_field<std::string>(product_json, "product_name");
        product->productType = product_json.at("type").get<std::string>();
        product->productSubtype = get_optional_field<std::string>(product_json, "subtype");

        product->nfrcid = get_optional_field<int>(product_json, "nfrc_id");
        product->cgdbShadingLayerId = get_optional_field<int>(product_json, "cgdb_id");
        product->cgdbShadeMaterialId = get_optional_field<int>(product_json, "cgdb_id");
        product->igdbDatabaseVersion =
          get_optional_field<std::string>(product_json, "igdb_database_version");
        product->cgdbDatabaseVersion =
          get_optional_field<std::string>(product_json, "cgdb_database_version");
        product->igdbChecksum = get_optional_field<int>(product_json, "igdb_checksum");
        product->cgdbChecksum = get_optional_field<int>(product_json, "cgdb_checksum");
        product->manufacturer = product_json.at("manufacturer").get<std::string>();
        product->material =
          get_optional_field<std::string>(product_json, "material_bulk_properties");

        product->coatingName = get_optional_field<std::string>(product_json, "coating_name");
        product->coatedSide = get_optional_field<std::string>(product_json, "coated_side");

        product->substrateFilename =
          get_optional_field<std::string>(product_json, "interlayer_name");


        product->appearance = get_optional_field<std::string>(product_json, "appearance");
        product->acceptance = get_optional_field<std::string>(product_json, "acceptance");
        product->fileName = get_optional_field<std::string>(product_json, "filename");
        product->dataFileName = get_optional_field<std::string>(product_json, "data_file_name");
        product->unitSystem = get_optional_field<std::string>(product_json, "unit_system");
        product->opticalOpenness = get_optional_field<double>(product_json, "optical_openness");

        nlohmann::json measured_data_json = product_json.at("physical_properties");

        product->thickness = get_optional_field<double>(measured_data_json, "thickness");
        product->conductivity = get_optional_field<double>(measured_data_json, "conductivity");
        product->IRTransmittance =
          get_optional_field<double>(measured_data_json, "predefined_tir_front");
        product->frontEmissivity =
          get_optional_field<double>(measured_data_json, "predefined_emissivity_front");
        product->backEmissivity =
          get_optional_field<double>(measured_data_json, "predefined_emissivity_back");

        product->frontEmissivitySource =
          get_optional_field<std::string>(measured_data_json, "emissivity_front_source");

        product->backEmissivitySource =
          get_optional_field<std::string>(measured_data_json, "emissivity_back_source");

        product->permeabilityFactor =
          get_optional_field<double>(measured_data_json, "permeability_factor");

        product->backEmissivitySource =
          get_optional_field<std::string>(measured_data_json, "wavelength_units");

        if(measured_data_json.find("power_properties") != measured_data_json.end())
        {
            product->pvPowerProperties =
              parsePowerProperties(measured_data_json.at("power_properties"));
        }

        nlohmann::json optical_properties_json = measured_data_json.at("optical_properties");
        nlohmann::json optical_data_json = optical_properties_json.at("optical_data");
        nlohmann::json angle_blocks_json = optical_data_json.at("angle_blocks");

        std::vector<WLData> measurements;
        for(auto & angle_block_json : angle_blocks_json)
        {
            nlohmann::json wavelength_data_json = angle_block_json.at("wavelength_data");
            for(auto & individual_wavelength_json : wavelength_data_json)
            {
                double wavelength =
                  std::stod(individual_wavelength_json.at("w").get<std::string>());
                std::optional<MeasurementComponent> specular;
                std::optional<MeasurementComponent> diffuse;
                std::optional<PVWavelengthData> pv;
                if(individual_wavelength_json.find("specular") != individual_wavelength_json.end())
                {
                    nlohmann::json specular_json = individual_wavelength_json.at("specular");
                    double tf = std::stod(specular_json.at("tf").get<std::string>());
                    double tb = std::stod(specular_json.at("tb").get<std::string>());
                    double rf = std::stod(specular_json.at("rf").get<std::string>());
                    double rb = std::stod(specular_json.at("rb").get<std::string>());
                    specular = MeasurementComponent{tf, tb, rf, rb};
                }
                if(individual_wavelength_json.find("diffuse") != individual_wavelength_json.end())
                {
                    nlohmann::json diffuse_json = individual_wavelength_json.at("diffuse");
                    double tf = std::stod(diffuse_json.at("tf").get<std::string>());
                    double tb = std::stod(diffuse_json.at("tb").get<std::string>());
                    double rf = std::stod(diffuse_json.at("rf").get<std::string>());
                    double rb = std::stod(diffuse_json.at("rb").get<std::string>());
                    diffuse = MeasurementComponent{tf, tb, rf, rb};
                }
                if(individual_wavelength_json.find("pv") != individual_wavelength_json.end())
                {
                    nlohmann::json pv_json = individual_wavelength_json.at("pv");
                    double eqef = std::stod(pv_json.at("eqef").get<std::string>());
                    double eqeb = std::stod(pv_json.at("eqeb").get<std::string>());
                    pv = PVWavelengthData{eqef, eqeb};
                }
                measurements.push_back(WLData(wavelength, specular, diffuse, pv));
            }
        }

        if(!measurements.empty())
        {
            product->measurements = measurements;
        }


#if 0
		if(!spectral_data_json.is_null())
		{
			nlohmann::json wavelength_data_json = spectral_data_json.at("spectral_data");

			std::vector<WLData> measurements;

			for(nlohmann::json::iterator itr = wavelength_data_json.begin();
				itr != wavelength_data_json.end();
				++itr)
			{
				auto val = itr.value();
				double wl = val.at("wavelength").get<double>();
				double t = val.at("T").get<double>();
				double rf = val.at("Rf").get<double>();
				double rb = val.at("Rb").get<double>();
				MeasurementComponent directValues{t, t, rf, rb};
				measurements.push_back(WLData(wl, directValues));
			}
			if(!measurements.empty())
			{
				product->measurements = measurements;
			}
			parseDualBandValues(product, spectral_data_json);
		}
		parseIntegratedResults(product, product_json);
#endif
        return product;
    }

    std::shared_ptr<ProductGeometry> parseVenetianGeometry(nlohmann::json const & geometry_json)
    {
        auto slatWidth = geometry_json.at("slat_width").get<double>();
        auto slatSpacing = geometry_json.at("slat_spacing").get<double>();
        auto slatCurvature = geometry_json.at("slat_curvature").get<double>();
        auto numberSegments = geometry_json.at("number_segments").get<int>();
        double slatTilt = geometry_json.value("slat_tilt", 0.0);
        std::string tiltChoice = geometry_json.at("tilt_choice").get<std::string>();

        // These values are stored as mm in the sources being parsed.
        // Convert to meters here for consistancy with other non-wavelength
        // length units.
        slatWidth /= 1000.0;
        slatSpacing /= 1000.0;
        slatCurvature /= 1000.0;

        return std::shared_ptr<ProductGeometry>(new VenetianGeometry(
          slatWidth, slatSpacing, slatCurvature, slatTilt, tiltChoice, numberSegments));
    }

#if 0
	
=======
OpticsParser::ProductData parseIGSDBJson(nlohmann::json const & product_json)
{
    OpticsParser::ProductData product;
    product.productName = product_json.at("name").get<std::string>();
    product.productType = product_json.at("type").get<std::string>();
    product.productSubtype = product_json.at("subtype").get<std::string>();
    product.nfrcid = get_optional_field<int>(product_json, "nfrc_id");
    product.igdbChecksum = get_optional_field<int>(product_json, "igdb_checksum");
    product.igdbDatabaseVersion =
      get_optional_field<std::string>(product_json, "igdb_database_version");
    product.manufacturer = product_json.at("manufacturer_name").get<std::string>();
    product.material = get_optional_field<std::string>(product_json, "material_bulk_properties");


    product.coatingName = get_optional_field<std::string>(product_json, "coating_name");
    product.coatedSide = get_optional_field<std::string>(product_json, "coated_side");
>>>>>>> origin/WINDOW_8_update_glass_from_IGSDB
#    endif

      std::shared_ptr
      < ProductGeometry> parseWovenGeometry(nlohmann::json const & geometry_json)
    {
        auto threadDiameter = geometry_json.at("thread_diameter").get<double>();
        auto threadSpacing = geometry_json.at("thread_spacing").get<double>();
        auto shadeThickness = geometry_json.at("shade_thickness").get<double>();

        return std::shared_ptr<ProductGeometry>(
          new WovenGeometry(threadDiameter, threadSpacing, shadeThickness));
    }

    std::shared_ptr<ProductGeometry> parsePerforatedGeometry(nlohmann::json const & geometry_json)
    {
        double spacingX = geometry_json.at("spacing_x").get<double>();
        double spacingY = geometry_json.at("spacing_y").get<double>();
        double dimensionX = geometry_json.at("dimension_x").get<double>();
        double dimensionY = geometry_json.at("dimension_y").get<double>();
        std::string perforationType = geometry_json.at("perforation_type").get<std::string>();

        return std::shared_ptr<ProductGeometry>(
          new PerforatedGeometry(spacingX, spacingY, dimensionX, dimensionY, perforationType));
    }

    std::shared_ptr<ProductGeometry> parseGeometry(std::string const & productSubtype,
                                                   nlohmann::json const & geometry_json)
    {
        std::map<std::string,
                 std::function<std::shared_ptr<ProductGeometry>(nlohmann::json const &)>>
          mapping;

        mapping["venetian"] = &parseVenetianGeometry;
        mapping["woven"] = &parseWovenGeometry;
        mapping["perforated-screen"] = &parsePerforatedGeometry;

        auto itr = mapping.find(productSubtype);
        if(itr != mapping.end())
        {
            return itr->second(geometry_json);
        }
        else
        {
            std::stringstream msg;
            msg << "Subtype " << productSubtype << " geometry not yet supported.";
            throw std::runtime_error(msg.str());
        }
    }

    std::shared_ptr<ProductData>
      parseIGSDBJsonComposedProduct_v1(nlohmann::json const & product_json)
    {
        auto subtype = product_json.at("subtype").get<std::string>();
        auto composition_information = product_json.at("composition");
        auto product_material = composition_information[0].at("child_product");
        auto product_geometry = composition_information[0].at("extra_data").at("geometry");
        auto material = parseIGSDBJsonUncomposedProduct_v1(product_material);
        auto geometry = parseGeometry(subtype, product_geometry);
        std::shared_ptr<CompositionInformation> compositionInformation(
          new CompositionInformation{material, geometry});
        auto product = parseIGSDBJsonUncomposedProduct_v1(product_json);
        std::shared_ptr<ProductData> composedProduct(
          new ComposedProductData(*product, compositionInformation));
        return composedProduct;
    }

    std::shared_ptr<ProductData> parseIGSDBJson_v1(nlohmann::json const & product_json)
    {
        if(product_json.count("composition") && !product_json.at("composition").empty())
        {
            return parseIGSDBJsonComposedProduct_v1(product_json);
        }
        else
        {
            return parseIGSDBJsonUncomposedProduct_v1(product_json);
        }
    }

    std::shared_ptr<ProductData> parseIGSDBJson_v2(nlohmann::json const & product_json)
    {
        if(product_json.count("composition") && !product_json.at("composition").empty())
        {
            throw std::runtime_error("Composed products in IGSDB v2 are not yet supported.");
        }
        else
        {
            return parseIGSDBJsonUncomposedProduct_v2(product_json);
        }
    }

    std::shared_ptr<ProductData> Parser::parseJSONString(std::string const & json_str)
    {
        nlohmann::json product_json = nlohmann::json::parse(json_str);

        // There are now two different json formats: IGSDB v1 and IGSDB v2
        // There are many different field names one of which is v1 has "product_id"
        // and v2 has "id".  Using that to detect which version is being handled
        if(product_json.find("product_id") != product_json.end())
        {
            return parseIGSDBJson_v1(product_json);
        }
        else
        {
            // Check to make sure there is an id field.  If it is missing both product_id
            // and id then it is some other format that is not currently supporte
            if(product_json.find("id") == product_json.end())
            {
                throw std::runtime_error("Unable to parse json.  It does not appear to be "
                                         "either of the two recognized formats.  Currently "
                                         "only IGSDB v1 and v2 formats are supported.");
            }
            return parseIGSDBJson_v2(product_json);
        }
    }


    std::shared_ptr<ProductData> Parser::parseJSONFile(std::string const & fname)
    {
        std::ifstream fin(fname);
        std::string content((std::istreambuf_iterator<char>(fin)),
                            (std::istreambuf_iterator<char>()));
        return parseJSONString(content);
    }

    std::shared_ptr<ProductData> parseOpticsFile(const std::string & inputFile)
    {
        Parser parser;
        return parser.parseFile(inputFile);
    }

    std::shared_ptr<ProductData> parseJSONString(std::string const & json)
    {
        Parser parser;
        return parser.parseJSONString(json);
    }

    std::shared_ptr<ProductData> parseJSONFile(std::string const & fname)
    {
        Parser parser;
        return parser.parseJSONFile(fname);
    }

    template<typename T, typename U>
    void assignOptionalValue(T & field, bool exists, U const & value)
    {
        if(exists)
        {
            field = value;
        }
    }

    char getSplitToken(std::string const & str)
    {
        char token = ',';   // Default to comma delmiter?
        std::vector<char> possibleTokens{',', ';', ' '};
        for(auto & tok : possibleTokens)
        {
            if(str.find(tok) != std::string::npos)
            {
                token = tok;
                break;
            }
        }
        return token;
    }

    std::vector<std::string> splitString(const std::string & str)
    {
        std::vector<std::string> tokens;
        std::istringstream test{str};
        char token = getSplitToken(str);
        std::string line;
        while(std::getline(test, line, token))
        {
            tokens.push_back(line);
        }

        return tokens;
    }

    std::vector<std::vector<double>> convertToSquareMatrix(std::vector<double> const & v)
    {
        double intPart;
        if(std::modf(std::sqrt(v.size()), &intPart) != 0)
        {
            throw std::runtime_error("Non-square matrix");
        }
        size_t size = static_cast<size_t>(intPart);
        std::vector<double> inner;
        inner.resize(size);
        std::vector<std::vector<double>> m(size, inner);
        for(size_t row = 0; row < size; ++row)
        {
            for(size_t col = 0; col < size; ++col)
            {
                m[row][col] = v[row * size + col];
            }
        }
        return m;
    }


    std::shared_ptr<ProductData> parseBSDFXML(XMLParser::XMLNode const & xWindowElementNode)
    {
        std::shared_ptr<ProductData> product(new ProductData());
        if(xWindowElementNode.isEmpty())
        {
            throw std::runtime_error("XML error : WindowElement not found");
        }
        XMLParser::XMLNode xLayerNode =
          xWindowElementNode.getChildNode("Optical").getChildNode("Layer");

        XMLParser::XMLNode matNode = xLayerNode.getChildNode("Material");
        product->productName = matNode.getChildNode("Name").getText();
        product->manufacturer = matNode.getChildNode("Manufacturer").getText();
        auto thickness = parseOptionalDoubleNode(matNode.getChildNode("Thickness"));
        auto thicknessUnitStr = matNode.getChildNode("Thickness").getAttribute("unit");
        if(toLower(thicknessUnitStr) == "millimeter")
        {}
        else if(thickness.has_value() && toLower(thicknessUnitStr) == "meter")
        {
            *thickness *= 1000.0;
        }
        else
        {
            throw std::runtime_error("XML error: Unsupported thickness unit");
        }
        product->thickness = thickness;
        product->frontEmissivity = parseOptionalDoubleNode(matNode.getChildNode("EmissivityFront"));
        product->backEmissivity = parseOptionalDoubleNode(matNode.getChildNode("EmissivityBack"));
        product->IRTransmittance = parseOptionalDoubleNode(matNode.getChildNode("TIR"));
        product->conductivity =
          parseOptionalDoubleNode(matNode.getChildNode("ThermalConductivity"));
        auto opennessNode = matNode.getChildNode("PermeabilityFactor");

        auto xEffectiveOpenness = matNode.getChildNode("EffectiveOpennessFraction");
        if(!xEffectiveOpenness.isEmpty() && opennessNode.isEmpty())
        {
            opennessNode = xEffectiveOpenness;
        }
        product->permeabilityFactor = parseOptionalDoubleNode(opennessNode);

        int wavelengthDataNodeCt = xLayerNode.nChildNode("WavelengthData");

        WavelengthBSDFs solarBSDFs;
        WavelengthBSDFs visibleBSDFs;

        for(int i = 0; i < wavelengthDataNodeCt; ++i)
        {
            XMLParser::XMLNode wavelengthDataNode = xLayerNode.getChildNode("WavelengthData", i);
            if(wavelengthDataNode.isEmpty())
                throw std::runtime_error("XML error: Empty WavelengthData section found");
            XMLParser::XMLNode wavelengthDataBlockNode =
              wavelengthDataNode.getChildNode("WavelengthDataBlock", 0);
            std::string wavelengthDirection =
              wavelengthDataBlockNode.getChildNode("WavelengthDataDirection").getText();
            XMLParser::XMLNode wavelengthNode = wavelengthDataNode.getChildNode("Wavelength");
            std::string wavelengthRange = wavelengthDataNode.getChildNode("Wavelength").getText();
            std::string wavelengthUnit =
              wavelengthDataNode.getChildNode("Wavelength").getAttribute("unit");

            if(wavelengthRange.empty())
            {
                throw std::runtime_error("XML error: Wavelength section not found");
            }
            if(wavelengthUnit.empty())
            {
                throw std::runtime_error("XML error: Wavelength unit not found");
            }
            if(toLower(wavelengthUnit) != "integral")
            {
                throw std::runtime_error(
                  "XML error: Only interal wavelength unit is currently supported");
            }

            WavelengthBSDFs * currentBandBSDFs;
            if(toLower(wavelengthRange) == "solar")
            {
                currentBandBSDFs = &solarBSDFs;
            }
            else if(toLower(wavelengthRange) == "visible")
            {
                currentBandBSDFs = &visibleBSDFs;
            }
            else
            {
                throw std::runtime_error("XML error: Unsupported wavelength range");
            }

            if(wavelengthDataBlockNode.isEmpty())
            {
                throw std::runtime_error("XML error: WavelengthDataBlock not found");
            }

            std::string dataString =
              wavelengthDataBlockNode.getChildNode("ScatteringData").getText();
            std::vector<std::string> splitDataString = splitString(dataString);
            std::vector<double> splitData;
            for(auto const & s : splitDataString)
            {
                splitData.push_back(std::stod(s));
            }
            std::vector<std::vector<double>> bsdf = convertToSquareMatrix(splitData);

            std::string columnAngleBasisName =
              wavelengthDataBlockNode.getChildNode("ColumnAngleBasis").getText();
            std::string rowAngleBasisName =
              wavelengthDataBlockNode.getChildNode("RowAngleBasis").getText();

            if(toLower(wavelengthDirection) == "transmission front")
            {
                currentBandBSDFs->tf = BSDF{bsdf, rowAngleBasisName, columnAngleBasisName};
            }
            else if(toLower(wavelengthDirection) == "transmission back")
            {
                currentBandBSDFs->tb = BSDF{bsdf, rowAngleBasisName, columnAngleBasisName};
            }
            else if(toLower(wavelengthDirection) == "reflection front")
            {
                currentBandBSDFs->rf = BSDF{bsdf, rowAngleBasisName, columnAngleBasisName};
            }
            else if(toLower(wavelengthDirection) == "reflection back")
            {
                currentBandBSDFs->rb = BSDF{bsdf, rowAngleBasisName, columnAngleBasisName};
            }
        }

        product->measurements = DualBandBSDF{solarBSDFs, visibleBSDFs};

        return product;
    }

    std::shared_ptr<ProductData> parseBSDFXMLString(std::string const & contents)
    {
        XMLParser::XMLNode xWindowElementNode =
          XMLParser::XMLNode::parseString(contents.c_str(), "WindowElement");
        return parseBSDFXML(xWindowElementNode);
    }

    std::shared_ptr<ProductData> parseBSDFXMLFile(std::string const & fname)
    {
        XMLParser::XMLNode xWindowElementNode =
          XMLParser::XMLNode::openFileHelper(fname.c_str(), "WindowElement");
        return parseBSDFXML(xWindowElementNode);
    }


}   // namespace OpticsParser
