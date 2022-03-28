#include <memory>
#include <gtest/gtest.h>
#include <memory>
#include <sstream>
#include <filesystem>

#include "Parser.hpp"

#include "paths.h"

extern std::string test_dir;

class TestLoadIGSDBJSONFromDisk : public testing::Test
{
protected:
    virtual void SetUp()
    {}
};


TEST_F(TestLoadIGSDBJSONFromDisk, TestLoadIGSDBVenetianShadingLayerJSON)
{
    SCOPED_TRACE("Begin Test: Load IGSDB venetian shading layer json format.");
    std::filesystem::path product_path(test_dir);
    product_path /= "products";
    product_path /= "igsdb_v1_venetian.json";

    OpticsParser::Parser parser;


    std::shared_ptr<OpticsParser::ComposedProductData> product =
      std::dynamic_pointer_cast<OpticsParser::ComposedProductData>(
        parser.parseJSONFile(product_path.string()));

    //    EXPECT_EQ(product->nfrcid.value(), 102);
    EXPECT_EQ(product->productName.value(), "Slim White Venetian Blind");
    EXPECT_EQ(product->productType, "shading");
    EXPECT_EQ(product->manufacturer, "Pella");
    EXPECT_EQ(product->thickness, std::optional<double>());
    // TODO EXPECT_EQ(product->conductivity, 1.0);
    EXPECT_EQ(product->IRTransmittance, std::optional<double>());
    EXPECT_EQ(product->frontEmissivity, std::optional<double>());
    EXPECT_EQ(product->backEmissivity, std::optional<double>());
    EXPECT_EQ(product->measurements.has_value(), false);
    auto geometry = std::dynamic_pointer_cast<OpticsParser::VenetianGeometry>(
      product->compositionInformation->geometry);
    EXPECT_EQ(geometry->slatWidth, 0.0148);
    EXPECT_EQ(geometry->slatSpacing, 0.0127);
    EXPECT_EQ(geometry->slatCurvature, 0.03313057);
    EXPECT_EQ(geometry->numberSegments, 5);
    auto material = product->compositionInformation->material;
    EXPECT_EQ(material->productName.value(), "White Venetian Blind Slat");
    EXPECT_EQ(material->productType, "material");
    EXPECT_EQ(material->manufacturer, "Pella");
    EXPECT_EQ(material->thickness, 0.1);
    EXPECT_EQ(material->IRTransmittance, 0.0);
    EXPECT_EQ(material->frontEmissivity, 0.9);
    EXPECT_EQ(material->backEmissivity, 0.9);
    EXPECT_EQ(material->measurements.has_value(), true);
    auto & materialMeasurements =
      std::get<std::vector<OpticsParser::WLData>>(material->measurements.value());
    EXPECT_EQ(materialMeasurements.size(), 441);
    EXPECT_EQ(materialMeasurements[0].wavelength, 0.300000011920929);
    EXPECT_EQ(materialMeasurements[0].directComponent.value().tf, 0.0);
    EXPECT_EQ(materialMeasurements[0].directComponent.value().rf, 0.0702999979257584);
    EXPECT_EQ(materialMeasurements[0].directComponent.value().rb, 0.0702999979257584);
    EXPECT_EQ(materialMeasurements[440].wavelength, 2.50);
    EXPECT_EQ(materialMeasurements[440].directComponent.value().tf, 0.0);
    EXPECT_EQ(materialMeasurements[440].directComponent.value().rf, 0.718999981880188);
    EXPECT_EQ(materialMeasurements[440].directComponent.value().rb, 0.718999981880188);
}

TEST_F(TestLoadIGSDBJSONFromDisk, TestLoadIGSDBPerforatedScreenShadingLayerJSON)
{
    SCOPED_TRACE("Begin Test: Load IGSDB perforated shading layer json format.");
    std::filesystem::path product_path(test_dir);
    product_path /= "products";
    product_path /= "igsdb_v1_perforated.json";

    OpticsParser::Parser parser;


    std::shared_ptr<OpticsParser::ComposedProductData> product =
      std::dynamic_pointer_cast<OpticsParser::ComposedProductData>(
        parser.parseJSONFile(product_path.string()));

    //    EXPECT_EQ(product->nfrcid.value(), 102);
    EXPECT_EQ(product->name, "Solar Comfort Radiant Barrier");
    EXPECT_EQ(product->productType, "shading");
    EXPECT_EQ(product->manufacturer, "Solar Comfort");
    EXPECT_EQ(product->thickness, std::optional<double>());
    // TODO EXPECT_EQ(product->conductivity, 1.0);
    EXPECT_EQ(product->IRTransmittance, std::optional<double>());
    EXPECT_EQ(product->frontEmissivity, std::optional<double>());
    EXPECT_EQ(product->backEmissivity, std::optional<double>());
    EXPECT_EQ(product->measurements.has_value(), false);
    auto geometry = std::dynamic_pointer_cast<OpticsParser::PerforatedGeometry>(
      product->compositionInformation->geometry);
    EXPECT_EQ(geometry->spacingX, 1.69);
    EXPECT_EQ(geometry->spacingY, 1.69);
    EXPECT_EQ(geometry->dimensionX, 0.58);
    EXPECT_EQ(geometry->dimensionY, 6.35);
    EXPECT_EQ(geometry->perforationType, "Circular");
    auto material = product->compositionInformation->material;
    EXPECT_EQ(material->name, "SCRadiantBarrier.txt");
    EXPECT_EQ(material->productType, "material");
    EXPECT_EQ(material->manufacturer, "Solar Comfort");
    EXPECT_EQ(material->thickness, 0.23);
    EXPECT_EQ(material->IRTransmittance, 0.0);
    EXPECT_EQ(material->frontEmissivity, 0.863);
    EXPECT_EQ(material->backEmissivity, 0.84);
    EXPECT_EQ(material->conductivity, 0.12);
    EXPECT_EQ(material->measurements.has_value(), true);
    auto & materialMeasurements =
      std::get<std::vector<OpticsParser::WLData>>(material->measurements.value());
    EXPECT_EQ(materialMeasurements.size(), 441);
    EXPECT_EQ(materialMeasurements[0].wavelength, 0.3);
    EXPECT_EQ(materialMeasurements[0].directComponent.value().tf, 0.0);
    EXPECT_EQ(materialMeasurements[0].directComponent.value().rf, 0.0794);
    EXPECT_EQ(materialMeasurements[0].directComponent.value().rb, 0.0674);
    EXPECT_EQ(materialMeasurements[440].wavelength, 2.50);
    EXPECT_EQ(materialMeasurements[440].directComponent.value().tf, 0.0);
    EXPECT_EQ(materialMeasurements[440].directComponent.value().rf, 0.6568);
    EXPECT_EQ(materialMeasurements[440].directComponent.value().rb, 0.48);
}
