#include <memory>
#include <gtest/gtest.h>
#include <memory>
#include <sstream>
//#include <filesystem>

#include "Parser.hpp"

#include "paths.h"

extern std::string test_dir;


class TestLoadOpticsFileFromDisk : public testing::Test
{
protected:
    virtual void SetUp()
    {}
};


TEST_F(TestLoadOpticsFileFromDisk, TestLoadClear3)
{
    SCOPED_TRACE("Begin Test: Load CLEAR_3.DAT from disk.");

    std::string clear_3_path(test_dir);
    clear_3_path += "/products";
    clear_3_path += "/CLEAR_3.DAT";

    OpticsParser::Parser parser;
    std::shared_ptr<OpticsParser::ProductData> product = parser.parseFile(clear_3_path);
    EXPECT_EQ(product->nfrcid.value(), 102);
    EXPECT_EQ(product->productName, "Generic Clear Glass");
    EXPECT_EQ("glazing", product->productType);
    EXPECT_EQ("Monolithic", product->productSubtype);
    EXPECT_EQ(product->thickness.value(), 3.048);
    EXPECT_EQ(product->conductivity, 1.0);
    EXPECT_EQ(product->IRTransmittance.value(), 0.0);
    EXPECT_EQ(product->frontEmissivity.value(), 0.84);
    EXPECT_EQ(product->backEmissivity.value(), 0.84);
    auto & givenResults =
      std::get<std::vector<OpticsParser::WLData>>(product->measurements.value());
    EXPECT_EQ(givenResults.size(), 111);
    EXPECT_EQ(givenResults[0].wavelength, 0.3);
    EXPECT_EQ(givenResults[0].directComponent.value().tf, 0.002);
    EXPECT_EQ(givenResults[0].directComponent.value().rf, 0.047);
    EXPECT_EQ(givenResults[0].directComponent.value().rb, 0.048);
    EXPECT_EQ(givenResults[110].wavelength, 2.5);
    EXPECT_EQ(givenResults[110].directComponent.value().tf, 0.822);
    EXPECT_EQ(givenResults[110].directComponent.value().rf, 0.068);
    EXPECT_EQ(givenResults[110].directComponent.value().rb, 0.068);
    EXPECT_EQ(product->frontEmissivitySource.value(), "Material");
    EXPECT_EQ(product->backEmissivitySource.value(), "Material");
    EXPECT_EQ(product->manufacturer, "Generic");
    EXPECT_EQ(product->material, "Glass");
    EXPECT_EQ(product->coatingName, "N/A");
    EXPECT_EQ(product->coatedSide, "Neither");
    EXPECT_EQ(product->substrateFilename, "N/A");
    EXPECT_EQ(product->appearance, "Clear");
    EXPECT_EQ(product->acceptance, "#");
    EXPECT_EQ(product->unitSystem, "SI");
    EXPECT_EQ(product->wavelengthUnit, "Microns");
}

TEST_F(TestLoadOpticsFileFromDisk, TestLoadDiffuseData)
{
    SCOPED_TRACE(
      "Begin Test: Load a file with diffuse data from disk (diffuse_optics_file_2.txt).");

    std::string product_path(test_dir);
    product_path += "/products";
    product_path += "/diffuse_optics_file_2.txt";

    OpticsParser::Parser parser;
    std::shared_ptr<OpticsParser::ProductData> product = parser.parseFile(product_path);
    EXPECT_EQ(product->nfrcid, std::optional<int>());
    EXPECT_EQ(product->productName, "Generic frit 38mm aperture");
    EXPECT_EQ("glazing", product->productType);
    EXPECT_EQ("Coated", product->productSubtype);
    EXPECT_EQ(product->thickness.value(), 6.0);
    EXPECT_EQ(product->conductivity, 1.0);
    EXPECT_EQ(product->IRTransmittance.value(), 0.0);
    EXPECT_EQ(product->frontEmissivity.value(), 0.86);
    EXPECT_EQ(product->backEmissivity.value(), 0.86);
    auto & givenMeasurements =
      std::get<std::vector<OpticsParser::WLData>>(product->measurements.value());
    EXPECT_EQ(givenMeasurements.size(), 451);
    EXPECT_EQ(givenMeasurements[0].wavelength, 0.250);
    EXPECT_EQ(givenMeasurements[0].directComponent.value().tf, 0.001);
    EXPECT_EQ(givenMeasurements[0].directComponent.value().tb, 0.006);
    EXPECT_EQ(givenMeasurements[0].directComponent.value().rf, 0.011);
    EXPECT_EQ(givenMeasurements[0].directComponent.value().rb, 0.006);
    EXPECT_EQ(givenMeasurements[0].diffuseComponent.value().tf, 0.002);
    EXPECT_EQ(givenMeasurements[0].diffuseComponent.value().tb, 0.076);
    EXPECT_EQ(givenMeasurements[0].diffuseComponent.value().rf, 0.087);
    EXPECT_EQ(givenMeasurements[0].diffuseComponent.value().rb, 0.131);
    EXPECT_EQ(givenMeasurements[450].wavelength, 2.5);
    EXPECT_EQ(givenMeasurements[450].directComponent.value().tf, 0.520);
    EXPECT_EQ(givenMeasurements[450].directComponent.value().tb, 0.528);
    EXPECT_EQ(givenMeasurements[450].directComponent.value().rf, 0.000);
    EXPECT_EQ(givenMeasurements[450].directComponent.value().rb, 0.000);
    EXPECT_EQ(givenMeasurements[450].diffuseComponent.value().tf, 0.238);
    EXPECT_EQ(givenMeasurements[450].diffuseComponent.value().tb, 0.240);
    EXPECT_EQ(givenMeasurements[450].diffuseComponent.value().rf, 0.0970);
    EXPECT_EQ(givenMeasurements[450].diffuseComponent.value().rb, 0.0940);
    EXPECT_EQ(product->frontEmissivitySource, std::optional<std::string>());
    EXPECT_EQ(product->backEmissivitySource, std::optional<std::string>());
    EXPECT_EQ(product->manufacturer, "LBNL demo");
    EXPECT_EQ(product->material, std::optional<std::string>());
    EXPECT_EQ(product->coatingName, "Generic clear frit");
    EXPECT_EQ(product->coatedSide, "Back");
    EXPECT_EQ(product->substrateFilename, "CLEAR_6.DAT");
    EXPECT_EQ(product->appearance, "Hazy");
    EXPECT_EQ(product->acceptance, std::optional<std::string>());
    EXPECT_EQ(product->unitSystem, "SI");
    EXPECT_EQ(product->wavelengthUnit, "Microns");
    EXPECT_EQ(product->permeabilityFactor, 0.0);
}

TEST_F(TestLoadOpticsFileFromDisk, TestMissingFile)
{
    SCOPED_TRACE("Begin Test: Load file that doesn't exist.");

    std::string clear_3_path(test_dir);
    clear_3_path += "/some_invalid_path";
    clear_3_path += "/CLEAR_3.DAT";

    EXPECT_THROW(OpticsParser::parseOpticsFile(clear_3_path), std::runtime_error);
}
