#include <memory>
#include <gtest/gtest.h>
#include <fstream>

#include "OpticsParser.hpp"

class TestInvertedEmissivities : public testing::Test
{
protected:
	void SetUp() override
	{
		std::string inputContent = "{ Units, Wavelength Units } SI Microns\n"
			"{ Thickness } 3.048 \n"
			"{ Conductivity } 1\n"
			"{ IR Transmittance } TIR=0\n"
			"{ Emissivity, back front } Emis= 0.5 0.84\n"
			"{ }\n"
			"{ Ef_Source: Material }\n"
			"{ Eb_Source: Material }\n"
			"{ IGDB_Checksum: -1717699038 }\n"
			"{ Product Name: }\n"
			"{ Manufacturer: Generic }\n"
			"{ NFRC ID: 102 }\n"
			"{ Type: Monolithic }\n"
			"{ Material: Glass }\n"
			"{ Coating Name: N/A }\n"
			"{ Coated Side: Neither }\n"
			"{ Substrate Filename: N/A }\n"
			"{ Appearance: Clear }\n"
			"{ Acceptance: # }\n"
			"{ Uses:  }\n"
			"{ Availability:   }\n"
			"{ Structure:  }\n"
			"0.300    0.0020    0.0470    0.0480\n"
			"0.305    0.0030    0.0470    0.0480\n"
			"0.310    0.0090    0.0470    0.0480\n"
			"0.315    0.0350    0.0470    0.0480\n"
			"0.320    0.1000    0.0470    0.0480\n"
			"0.325    0.2180    0.0490    0.0500";

		std::ofstream out("InputInvertedEmissivites.dat");
		out << inputContent;
		out.close();
	}
};

TEST_F(TestInvertedEmissivities, Test1)
{
	const std::string inputFile = R"(InputInvertedEmissivites.dat)";
	OpticsParser::Parser parser;
	std::shared_ptr<OpticsParser::ProductData> product = parser.parseFile(inputFile);

	EXPECT_NEAR(3.048, product->thickness.value(), 1e-6);
	EXPECT_NEAR(1, product->conductivity.value(), 1e-6);
	EXPECT_NEAR(0, product->IRTransmittance.value(), 1e-6);
	EXPECT_NEAR(0.84, product->frontEmissivity.value(), 1e-6);
	EXPECT_NEAR(0.5, product->backEmissivity.value(), 1e-6);
	EXPECT_EQ(102, product->nfrcid.value());
	EXPECT_EQ("", product->productName);
	EXPECT_EQ("glazing", product->productType);
	EXPECT_EQ("Monolithic", product->productSubtype);

	std::vector<OpticsParser::WLData> correctResults{{0.300, 0.0020, 0.0470, 0.0480},
													 {0.305, 0.0030, 0.0470, 0.0480},
													 {0.310, 0.0090, 0.0470, 0.0480},
													 {0.315, 0.0350, 0.0470, 0.0480},
													 {0.320, 0.1000, 0.0470, 0.0480},
													 {0.325, 0.2180, 0.0490, 0.0500}};

	auto & givenResults =
		std::get<std::vector<OpticsParser::WLData>>(product->measurements.value());
	EXPECT_EQ(correctResults.size(), givenResults.size());
	for(auto i = 0u; i < correctResults.size(); ++i)
	{
		EXPECT_NEAR(correctResults[i].wavelength, givenResults[i].wavelength, 1e-6);
		EXPECT_NEAR(
			correctResults[i].directComponent.value().tf, givenResults[i].directComponent.value().tf, 1e-6);
		EXPECT_NEAR(
			correctResults[i].directComponent.value().rf, givenResults[i].directComponent.value().rf, 1e-6);
		EXPECT_NEAR(
			correctResults[i].directComponent.value().rb, givenResults[i].directComponent.value().rb, 1e-6);
	}
}