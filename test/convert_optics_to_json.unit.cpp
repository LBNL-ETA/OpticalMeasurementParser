#include <memory>
#include <gtest/gtest.h>
#include <memory>
#include <sstream>
//#include <filesystem>

#include "Parser.hpp"

#include "paths.h"

extern std::string test_dir;


class TestConvertOpticsFile : public testing::Test
{
protected:
    virtual void SetUp()
    {}
};


TEST_F(TestConvertOpticsFile, TestConveretClear3)
{
    SCOPED_TRACE("Begin Test: Convert CLEAR_3.DAT to json.");

    std::string clear_3_path(test_dir);
    clear_3_path += "/products";
    clear_3_path += "/CLEAR_3.DAT";

    OpticsParser::Parser parser;
    OpticsParser::ProductData product = parser.parseFile(clear_3_path);
    nlohmann::json product_json = product;
    
    EXPECT_EQ(product_json.at("nfrc_id").get<int>(), 102);
    EXPECT_EQ(product_json.at("name").get<std::string>(), "Generic Clear Glass");
    EXPECT_EQ(product_json.at("product_type").get<std::string>(), "Monolithic");
    EXPECT_EQ(product_json.at("measured_data").at("thickness").get<double>(), 3.048);
    EXPECT_EQ(product_json.at("measured_data").at("material_bulk_properties_overrides").at("thermal_conductivity").get<double>(), 1.0);
    EXPECT_EQ(product_json.at("measured_data").at("tir_front").get<double>(), 0.0);
    EXPECT_EQ(product_json.at("measured_data").at("emissivity_front").get<double>(), 0.84);
    EXPECT_EQ(product_json.at("measured_data").at("emissivity_back").get<double>(), 0.84);

    nlohmann::json spectral_data = product_json.at("measured_data").at("spectral_data");
    //EXPECT_EQ(spectral_data.at("unit").get<std::string>(), "nanometer");
    EXPECT_EQ(spectral_data.at("number_incidence_angles").get<int>(), 1);

    nlohmann::json angle_block = spectral_data.at("angle_block")[0];

    EXPECT_EQ(angle_block.at("incidence_angle").get<int>(), 0);
    EXPECT_EQ(angle_block.at("number_wavelengths").get<int>(), 111);

    nlohmann::json measurements = angle_block.at("solar_optical_spectral_data");


    EXPECT_EQ(measurements[0].at("w").get<double>(), 0.3);
    EXPECT_EQ(measurements[0].at("tf").get<double>(), 0.002);
    EXPECT_EQ(measurements[0].at("rf").get<double>(), 0.047);
    EXPECT_EQ(measurements[0].at("rb").get<double>(), 0.048);
    EXPECT_EQ(measurements[110].at("w").get<double>(), 2.5);
    EXPECT_EQ(measurements[110].at("tf").get<double>(), 0.822);
    EXPECT_EQ(measurements[110].at("rf").get<double>(), 0.068);
    EXPECT_EQ(measurements[110].at("rb").get<double>(), 0.068);

/*
    EXPECT_EQ(product.frontEmissivitySource, "Material");
    EXPECT_EQ(product.backEmissivitySource, "Material");
*/

    EXPECT_EQ(product_json.at("manufacturer").get<std::string>(), "Generic");
    EXPECT_EQ(product_json.at("type").get<std::string>(), "Glazing");
    EXPECT_EQ(product_json.at("specularity").get<std::string>(), "Specular");
    EXPECT_EQ(product_json.at("coating_properties").at("coating_name").get<std::string>(), "N/A");
    EXPECT_EQ(product_json.at("coating_properties").at("coated_side").get<std::string>(), "Neither");
//    EXPECT_EQ(product_json.at("substrate_filename").get<std::string>(), "N/A");
    EXPECT_EQ(product_json.at("appearance").get<std::string>(), "Clear");
    EXPECT_EQ(product_json.at("acceptance").get<std::string>(), "#");

}
