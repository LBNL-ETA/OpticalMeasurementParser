#include <memory>
#include <gtest/gtest.h>
#include <memory>
#include <sstream>
#include <filesystem>

#include "Parser.hpp"

#include "paths.h"

extern std::string test_dir;

#if 1
class TestLoadJSONFromDisk : public testing::Test
{
protected:
	virtual void SetUp()
	{}
};


TEST_F(TestLoadJSONFromDisk, TestLoadClear3JSON)
{
	SCOPED_TRACE("Begin Test: Load NFRC 2003 from disk.");

	SCOPED_TRACE("Begin Test: Load CLEAR_3.DAT from disk.");

	std::filesystem::path clear_3_path(test_dir);
	clear_3_path /= "products";
	clear_3_path /= "CLEAR_3.json";

	std::shared_ptr<OpticsParser::ProductData> product = OpticsParser::parseJSONFile(clear_3_path.string());
	//    EXPECT_EQ(product->nfrcid.value(), 102);
	EXPECT_EQ(product->productName, "Generic Clear Glass");
	EXPECT_EQ(product->productType, "glazing");
	EXPECT_EQ(product->subtype, "monolithic");
	EXPECT_NEAR(product->thickness.value(), 3.048, 1e-6);
	EXPECT_EQ(product->conductivity, 1.0);
	EXPECT_EQ(product->IRTransmittance.value(), 0.0);
	EXPECT_NEAR(product->frontEmissivity.value(), 0.84, 1e-6);
	EXPECT_NEAR(product->backEmissivity.value(), 0.84, 1e-6);
	auto & measurements = std::get<std::vector<OpticsParser::WLData>>(product->measurements.value());
	EXPECT_EQ(measurements.size(), 111);
	EXPECT_NEAR(measurements[0].wavelength, 0.3, 1e-6);
	EXPECT_NEAR(measurements[0].directComponent.tf, 0.002, 1e-6);
	EXPECT_NEAR(measurements[0].directComponent.rf, 0.047, 1e-6);
	EXPECT_NEAR(measurements[0].directComponent.rb, 0.048, 1e-6);
	EXPECT_NEAR(measurements[110].wavelength, 2.5, 1e-6);
	EXPECT_NEAR(measurements[110].directComponent.tf, 0.822, 1e-6);
	EXPECT_NEAR(measurements[110].directComponent.rf, 0.068, 1e-6);
	EXPECT_NEAR(measurements[110].directComponent.rb, 0.068, 1e-6);
}
#endif