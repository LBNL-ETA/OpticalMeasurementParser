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
    product_path /= "igsdb_12149.json";

    OpticsParser::Parser parser;


    std::shared_ptr<OpticsParser::ComposedProductData> product = std::dynamic_pointer_cast<OpticsParser::ComposedProductData>(parser.parseJSONFile(product_path.string()));

    //    EXPECT_EQ(product->nfrcid.value(), 102);
    EXPECT_EQ(product->productName, "Slim White Venetian Blind");
    EXPECT_EQ(product->productType, "shading");
    EXPECT_EQ(product->manufacturer, "Pella");
    EXPECT_EQ(product->thickness, std::optional<double>());
    // TODO EXPECT_EQ(product->conductivity, 1.0);
    EXPECT_EQ(product->IRTransmittance, std::optional<double>());
    EXPECT_EQ(product->frontEmissivity, std::optional<double>());
    EXPECT_EQ(product->backEmissivity, std::optional<double>());
    EXPECT_EQ(product->measurements.has_value(), false);
	auto geometry = std::dynamic_pointer_cast<OpticsParser::VenetianGeometry>(product->compositionInformation->geometry);
	EXPECT_EQ(geometry->slatWidth, 14.8);
	EXPECT_EQ(geometry->slatSpacing, 12.7);
	EXPECT_EQ(geometry->slatCurvature, 33.13057);
	EXPECT_EQ(geometry->numberSegments, 5);
	auto material = product->compositionInformation->material;
	EXPECT_EQ(material->productName, "White Venetian Blind Slat (white.txt)");
    EXPECT_EQ(material->productType, "material");
    EXPECT_EQ(material->manufacturer, "Pella");
    EXPECT_EQ(material->thickness, 0.1);
    EXPECT_EQ(material->IRTransmittance, 0.0);
    EXPECT_EQ(material->frontEmissivity, 0.9);
    EXPECT_EQ(material->backEmissivity, 0.9);
    EXPECT_EQ(material->measurements.has_value(), true);

	EXPECT_EQ(material->measurements.value().size(), 441);
    EXPECT_EQ(material->measurements.value()[0].wavelength, 0.3);
    EXPECT_EQ(material->measurements.value()[0].directComponent.tf, 0.0);
    EXPECT_EQ(material->measurements.value()[0].directComponent.rf, 0.0703);
    EXPECT_EQ(material->measurements.value()[0].directComponent.rb, 0.0703);
    EXPECT_EQ(material->measurements.value()[440].wavelength, 2.50);
    EXPECT_EQ(material->measurements.value()[440].directComponent.tf, 0.0);
    EXPECT_EQ(material->measurements.value()[440].directComponent.rf, 0.719);
    EXPECT_EQ(material->measurements.value()[440].directComponent.rb, 0.719);

}

TEST_F(TestLoadIGSDBJSONFromDisk, TestLoadIGSDBPerforatedScreenShadingLayerJSON)
{
    SCOPED_TRACE("Begin Test: Load IGSDB perforated shading layer json format.");
    std::filesystem::path product_path(test_dir);
    product_path /= "products";
    product_path /= "igsdb_12295.json";

    OpticsParser::Parser parser;


    std::shared_ptr<OpticsParser::ComposedProductData> product =
      std::dynamic_pointer_cast<OpticsParser::ComposedProductData>(
        parser.parseJSONFile(product_path.string()));

    //    EXPECT_EQ(product->nfrcid.value(), 102);
    EXPECT_EQ(product->productName, "Solar Comfort Radiant Barrier");
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
    EXPECT_EQ(material->productName, "SCRadiantBarrier.txt");
    EXPECT_EQ(material->productType, "material");
    EXPECT_EQ(material->manufacturer, "Solar Comfort");
    EXPECT_EQ(material->thickness, 0.23);
    EXPECT_EQ(material->IRTransmittance, 0.0);
    EXPECT_EQ(material->frontEmissivity, 0.863);
    EXPECT_EQ(material->backEmissivity, 0.84);
    EXPECT_EQ(material->conductivity, 0.12);
    EXPECT_EQ(material->measurements.has_value(), true);

    EXPECT_EQ(material->measurements.value().size(), 441);
    EXPECT_EQ(material->measurements.value()[0].wavelength, 0.3);
    EXPECT_EQ(material->measurements.value()[0].directComponent.tf, 0.0);
    EXPECT_EQ(material->measurements.value()[0].directComponent.rf, 0.0794);
    EXPECT_EQ(material->measurements.value()[0].directComponent.rb, 0.0674);
    EXPECT_EQ(material->measurements.value()[440].wavelength, 2.50);
    EXPECT_EQ(material->measurements.value()[440].directComponent.tf, 0.0);
    EXPECT_EQ(material->measurements.value()[440].directComponent.rf, 0.6568);
    EXPECT_EQ(material->measurements.value()[440].directComponent.rb, 0.48);
}
