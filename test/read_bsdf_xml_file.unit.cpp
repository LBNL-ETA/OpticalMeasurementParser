#include <memory>
#include <gtest/gtest.h>
#include <memory>
#include <sstream>
#include <filesystem>

#include "Parser.hpp"

#include "paths.h"

extern std::string test_dir;

class TestLoadBSDFXMLFromDisk : public testing::Test
{
protected:
    virtual void SetUp()
    {}
};


TEST_F(TestLoadBSDFXMLFromDisk, TestLoad2011SA1)
{
    SCOPED_TRACE("Begin Test: Load 2011-SA1.XML");
    std::filesystem::path product_path(test_dir);
    product_path /= "products";
    product_path /= "2011-SA1.XML";

	std::shared_ptr<OpticsParser::ProductData> product =
		OpticsParser::parseBSDFXMLFile(product_path.string());

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

    
}
