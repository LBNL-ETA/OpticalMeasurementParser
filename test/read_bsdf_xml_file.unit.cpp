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
	EXPECT_EQ(product->productName, "Satine 5500 5%, White Pearl");
	EXPECT_EQ(product->productType, "");
	EXPECT_EQ(product->manufacturer, "Nysan");
	EXPECT_EQ(product->thickness, 1);
	EXPECT_EQ(product->conductivity, .15);
	EXPECT_EQ(product->IRTransmittance, 0.10916);
	EXPECT_EQ(product->frontEmissivity, 0.79626);
	EXPECT_EQ(product->backEmissivity, 0.79626);
	EXPECT_TRUE(product->measurements.has_value());
	EXPECT_TRUE(std::holds_alternative<OpticsParser::MultiBandBSDF>(product->measurements.value()));
	auto & measurements = std::get<OpticsParser::MultiBandBSDF>(product->measurements.value());
	auto & solar = measurements.at("solar");
	auto & visible = measurements.at("visible");
	EXPECT_EQ(solar.tf.size(), 145);
	EXPECT_EQ(solar.tb.size(), 145);
	EXPECT_EQ(solar.rf.size(), 145);
	EXPECT_EQ(solar.rb.size(), 145);
	EXPECT_EQ(visible.tf.size(), 145);
	EXPECT_EQ(visible.tb.size(), 145);
	EXPECT_EQ(visible.rf.size(), 145);
	EXPECT_EQ(visible.rb.size(), 145);
	for(size_t i = 0; i < solar.tf.size(); ++i)
	{
		EXPECT_EQ(solar.tf[i].size(), 145);
		EXPECT_EQ(solar.tb[i].size(), 145);
		EXPECT_EQ(solar.rf[i].size(), 145);
		EXPECT_EQ(solar.rb[i].size(), 145);
		EXPECT_EQ(visible.tf[i].size(), 145);
		EXPECT_EQ(visible.tb[i].size(), 145);
		EXPECT_EQ(visible.rf[i].size(), 145);
		EXPECT_EQ(visible.rb[i].size(), 145);
	}

	EXPECT_EQ(solar.tf[1][1], 2.009060);
	EXPECT_EQ(solar.tb[0][1], 0.021508);
	EXPECT_EQ(solar.rf[1][0], 0.148154);
	EXPECT_EQ(solar.rb[0][0], 0.167364);
	EXPECT_EQ(visible.tf[1][1], 2.027935);
	EXPECT_EQ(visible.tb[0][1], 0.014274);
	EXPECT_EQ(visible.rf[1][0], 0.153571);
	EXPECT_EQ(visible.rb[0][0], 0.178254);



}