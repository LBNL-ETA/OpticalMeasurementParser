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
	EXPECT_TRUE(std::holds_alternative<OpticsParser::DualBandBSDF>(product->measurements.value()));
	auto & measurements = std::get<OpticsParser::DualBandBSDF>(product->measurements.value());
	auto & solar = measurements.solar;
	auto & visible = measurements.visible;
	EXPECT_EQ(solar.tf.rowAngleBasisName, "LBNL/Klems Full");
	EXPECT_EQ(solar.tf.columnAngleBasisName, "LBNL/Klems Full");
	EXPECT_EQ(solar.tb.rowAngleBasisName, "LBNL/Klems Full");
	EXPECT_EQ(solar.tb.columnAngleBasisName, "LBNL/Klems Full");
	EXPECT_EQ(solar.rf.rowAngleBasisName, "LBNL/Klems Full");
	EXPECT_EQ(solar.rf.columnAngleBasisName, "LBNL/Klems Full");
	EXPECT_EQ(solar.rb.rowAngleBasisName, "LBNL/Klems Full");
	EXPECT_EQ(solar.rb.columnAngleBasisName, "LBNL/Klems Full");
	EXPECT_EQ(visible.tf.rowAngleBasisName, "LBNL/Klems Full");
	EXPECT_EQ(visible.tf.columnAngleBasisName, "LBNL/Klems Full");
	EXPECT_EQ(visible.tb.rowAngleBasisName, "LBNL/Klems Full");
	EXPECT_EQ(visible.tb.columnAngleBasisName, "LBNL/Klems Full");
	EXPECT_EQ(visible.rf.rowAngleBasisName, "LBNL/Klems Full");
	EXPECT_EQ(visible.rf.columnAngleBasisName, "LBNL/Klems Full");
	EXPECT_EQ(visible.rb.rowAngleBasisName, "LBNL/Klems Full");
	EXPECT_EQ(visible.rb.columnAngleBasisName, "LBNL/Klems Full");
	EXPECT_EQ(solar.tf.data.size(), 145);
	EXPECT_EQ(solar.tb.data.size(), 145);
	EXPECT_EQ(solar.rf.data.size(), 145);
	EXPECT_EQ(solar.rb.data.size(), 145);
	EXPECT_EQ(visible.tf.data.size(), 145);
	EXPECT_EQ(visible.tb.data.size(), 145);
	EXPECT_EQ(visible.rf.data.size(), 145);
	EXPECT_EQ(visible.rb.data.size(), 145);
	for(size_t i = 0; i < solar.tf.data.size(); ++i)
	{
		EXPECT_EQ(solar.tf.data[i].size(), 145);
		EXPECT_EQ(solar.tb.data[i].size(), 145);
		EXPECT_EQ(solar.rf.data[i].size(), 145);
		EXPECT_EQ(solar.rb.data[i].size(), 145);
		EXPECT_EQ(visible.tf.data[i].size(), 145);
		EXPECT_EQ(visible.tb.data[i].size(), 145);
		EXPECT_EQ(visible.rf.data[i].size(), 145);
		EXPECT_EQ(visible.rb.data[i].size(), 145);
	}

	EXPECT_EQ(solar.tf.data[1][1], 2.009060);
	EXPECT_EQ(solar.tb.data[0][1], 0.021508);
	EXPECT_EQ(solar.rf.data[1][0], 0.148154);
	EXPECT_EQ(solar.rb.data[0][0], 0.167364);
	EXPECT_EQ(visible.tf.data[1][1], 2.027935);
	EXPECT_EQ(visible.tb.data[0][1], 0.014274);
	EXPECT_EQ(visible.rf.data[1][0], 0.153571);
	EXPECT_EQ(visible.rb.data[0][0], 0.178254);

}