#include <memory>
#include <gtest/gtest.h>
#include <memory>
#include <sstream>
#include <filesystem>

#include "Parser.hpp"

#include "paths.h"

extern std::string test_dir;

class TestLoadJSONFromDisk : public testing::Test
{
protected:
    virtual void SetUp()
    {}
};


TEST_F(TestLoadJSONFromDisk, TestLoadCheckerToolJSON)
{
    SCOPED_TRACE("Begin Test: Load checker tool json format.");
    std::filesystem::path product_path(test_dir);
    product_path /= "products";
    product_path /= "checker_tool_input_example.json";

    OpticsParser::Parser parser;

    std::shared_ptr<OpticsParser::ProductData> product = parser.parseJSONFile(product_path.string());
    //    EXPECT_EQ(product->nfrcid.value(), 102);
    EXPECT_EQ(product->productName, "CGD89_092661");
    EXPECT_EQ(product->productType, "coated-glass");
    EXPECT_EQ(product->coatingName, "CGD89_092661");
    EXPECT_EQ(product->coatedSide, "Both");
    EXPECT_EQ(product->manufacturer, "Cardinal Glass Industries");
    EXPECT_EQ(product->thickness.value(), 2.2);
    // TODO EXPECT_EQ(product->conductivity, 1.0);
    EXPECT_EQ(product->IRTransmittance.value(), 0.0);
    EXPECT_EQ(product->frontEmissivity.value(), 0.149);
    EXPECT_EQ(product->backEmissivity.value(), 0.149);
    EXPECT_EQ(product->measurements.value().size(), 462);
    EXPECT_EQ(product->measurements.value()[0].wavelength, 0.3);
    EXPECT_EQ(product->measurements.value()[0].directComponent.tf, 0.0);
    EXPECT_EQ(product->measurements.value()[0].directComponent.rf, 0.021);
    EXPECT_EQ(product->measurements.value()[0].directComponent.rb, 0.021);
    EXPECT_EQ(product->measurements.value()[461].wavelength, 25.0);
    EXPECT_EQ(product->measurements.value()[461].directComponent.tf, 0.0);
    EXPECT_EQ(product->measurements.value()[461].directComponent.rf, 0.8894);
    EXPECT_EQ(product->measurements.value()[461].directComponent.rb, 0.8894);
}
