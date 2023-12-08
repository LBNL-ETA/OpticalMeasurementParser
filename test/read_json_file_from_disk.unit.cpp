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
	SCOPED_TRACE("Begin Test: Load CLEAR_3.json from disk.");

	std::filesystem::path clear_3_path(test_dir);
	clear_3_path /= "products";
	clear_3_path /= "igsdb_v1_clear_3.json";

	auto product = OpticsParser::parseJSONFile(clear_3_path.string());
	//    EXPECT_EQ(product.nfrcid.value(), 102);
	EXPECT_EQ(product.name, "Generic Clear Glass");
	EXPECT_EQ(product.productType, "glazing");
	EXPECT_EQ(product.productSubtype, "monolithic");
	EXPECT_NEAR(product.thickness.value(), 3.048, 1e-6);
	EXPECT_EQ(product.conductivity, 1.0);
	EXPECT_EQ(product.IRTransmittance.value(), 0.0);
	EXPECT_NEAR(product.frontEmissivity.value(), 0.84, 1e-6);
	EXPECT_NEAR(product.backEmissivity.value(), 0.84, 1e-6);
	auto & measurements = std::get<std::vector<OpticsParser::WLData>>(product.measurements.value());
	EXPECT_EQ(measurements.size(), 111);
	EXPECT_NEAR(measurements[0].wavelength, 0.3, 1e-6);
	EXPECT_NEAR(measurements[0].directComponent.value().tf, 0.002, 1e-6);
	EXPECT_NEAR(measurements[0].directComponent.value().rf, 0.047, 1e-6);
	EXPECT_NEAR(measurements[0].directComponent.value().rb, 0.048, 1e-6);
	EXPECT_NEAR(measurements[110].wavelength, 2.5, 1e-6);
	EXPECT_NEAR(measurements[110].directComponent.value().tf, 0.822, 1e-6);
	EXPECT_NEAR(measurements[110].directComponent.value().rf, 0.068, 1e-6);
	EXPECT_NEAR(measurements[110].directComponent.value().rb, 0.068, 1e-6);
}
#endif

#if 1

TEST_F(TestLoadJSONFromDisk, TestLoadClear3IGSDBV2JSON)
{
	SCOPED_TRACE("Begin Test: Load CLEAR_3_IGSDB_v2.json from disk.");

	std::filesystem::path clear_3_path(test_dir);
	clear_3_path /= "products";
	clear_3_path /= "igsdb_v2_clear_3.json";

	auto product = OpticsParser::parseJSONFile(clear_3_path.string());
	//    EXPECT_EQ(product.nfrcid.value(), 102);
	EXPECT_EQ(product.name, "Generic Clear Glass");
	EXPECT_EQ(product.productType, "GLAZING");
	EXPECT_EQ(product.productSubtype, "MONOLITHIC");
	EXPECT_NEAR(product.thickness.value(), 3.048, 1e-6);
	EXPECT_EQ(product.conductivity, std::optional<double>());
	EXPECT_EQ(product.IRTransmittance.value(), 0.0);
	EXPECT_NEAR(product.frontEmissivity.value(), 0.84, 1e-6);
	EXPECT_NEAR(product.backEmissivity.value(), 0.84, 1e-6);
	auto & measurements = std::get<std::vector<OpticsParser::WLData>>(product.measurements.value());
	EXPECT_EQ(measurements.size(), 111);
	EXPECT_NEAR(measurements[0].wavelength, 0.3, 1e-6);
	EXPECT_NEAR(measurements[0].directComponent.value().tf, 0.002, 1e-6);
	EXPECT_NEAR(measurements[0].directComponent.value().rf, 0.047, 1e-6);
	EXPECT_NEAR(measurements[0].directComponent.value().rb, 0.048, 1e-6);
	EXPECT_NEAR(measurements[110].wavelength, 2.5, 1e-6);
	EXPECT_NEAR(measurements[110].directComponent.value().tf, 0.822, 1e-6);
	EXPECT_NEAR(measurements[110].directComponent.value().rf, 0.068, 1e-6);
	EXPECT_NEAR(measurements[110].directComponent.value().rb, 0.068, 1e-6);
}

TEST_F(TestLoadJSONFromDisk, TestLoadPVIGSDBV2JSON)
{
	SCOPED_TRACE("Begin Test: Load igsdb_v2_pv.json from disk.");

	std::filesystem::path product_path(test_dir);
	product_path /= "products";
	product_path /= "generic_pv.json";

	auto product = OpticsParser::parseJSONFile(product_path.string());
	//    EXPECT_EQ(product.nfrcid.value(), 102);
	EXPECT_EQ(product.name, "Generic PV");
	EXPECT_EQ(product.productType, "GLAZING");
	EXPECT_EQ(product.productSubtype, "MONOLITHIC");
	EXPECT_NEAR(product.thickness.value(), 3.048, 1e-6);
	EXPECT_EQ(product.conductivity.value(), 1.0);
	EXPECT_EQ(product.IRTransmittance.value(), 0.0);
	EXPECT_NEAR(product.frontEmissivity.value(), 0.84, 1e-6);
	EXPECT_NEAR(product.backEmissivity.value(), 0.84, 1e-6);
	auto & measurements = std::get<std::vector<OpticsParser::WLData>>(product.measurements.value());
	EXPECT_EQ(measurements.size(), 441);
	EXPECT_NEAR(measurements[0].wavelength, 0.3, 1e-6);
	EXPECT_NEAR(measurements[0].directComponent.value().tf, 0.2, 1e-6);
	EXPECT_NEAR(measurements[0].directComponent.value().tb, 0.2, 1e-6);
	EXPECT_NEAR(measurements[0].directComponent.value().rf, 0.029467085000000000, 1e-6);
	EXPECT_NEAR(measurements[0].directComponent.value().rb, 0.023573667999999999, 1e-6);
	EXPECT_NEAR(measurements[0].pvComponent.value().eqef, 0.51039777099999994, 1e-6);
	EXPECT_NEAR(measurements[0].pvComponent.value().eqeb, 0.0, 1e-6);
	EXPECT_NEAR(measurements[110].wavelength, 0.85, 1e-6);
	EXPECT_NEAR(measurements[110].directComponent.value().tf, 0.2, 1e-6);
	EXPECT_NEAR(measurements[110].directComponent.value().tb, 0.2, 1e-6);
	EXPECT_NEAR(measurements[110].directComponent.value().rf, 0.011473822000000000, 1e-6);
	EXPECT_NEAR(measurements[110].directComponent.value().rb, 0.0091790580000000004, 1e-6);
	EXPECT_NEAR(measurements[110].pvComponent.value().eqef, 0.67389160100000001, 1e-6);
	EXPECT_NEAR(measurements[110].pvComponent.value().eqeb, 0.0, 1e-6);
	EXPECT_NEAR(measurements[440].wavelength, 2.5, 1e-6);
	EXPECT_NEAR(measurements[440].directComponent.value().tf, 0.2, 1e-6);
	EXPECT_NEAR(measurements[440].directComponent.value().tb, 0.2, 1e-6);
	EXPECT_NEAR(measurements[440].directComponent.value().rf, 0.036999999999999998, 1e-6);
	EXPECT_NEAR(measurements[440].directComponent.value().rb, 0.029600000000000001, 1e-6);
	EXPECT_NEAR(measurements[440].pvComponent.value().eqef, 0.0, 1e-6);
	EXPECT_NEAR(measurements[440].pvComponent.value().eqeb, 0.0, 1e-6);

	auto & allPowerProperties = product.pvPowerProperties.value();
	EXPECT_EQ(allPowerProperties.size(), 1);
	auto & powerProperties = allPowerProperties.at(298.15);
	EXPECT_EQ(powerProperties.size(), 400);
	EXPECT_NEAR(powerProperties[0].jsc, 0.1, 1e-9);
	EXPECT_NEAR(powerProperties[0].voc, 0.025138331000000000, 1e-9);
	EXPECT_NEAR(powerProperties[0].ff, 0.0, 1e-6);
	EXPECT_NEAR(powerProperties[160].jsc, 16.1, 1e-9);
	EXPECT_NEAR(powerProperties[160].voc, 0.56188651099999998, 1e-9);
	EXPECT_NEAR(powerProperties[160].ff, 0.66462276499999995, 1e-6);
	EXPECT_NEAR(powerProperties[399].jsc, 39.999999989999999, 1e-9);
	EXPECT_NEAR(powerProperties[399].voc, 0.60447729800000005, 1e-9);
	EXPECT_NEAR(powerProperties[399].ff, 0.71854149800000000, 1e-6);
}
#endif