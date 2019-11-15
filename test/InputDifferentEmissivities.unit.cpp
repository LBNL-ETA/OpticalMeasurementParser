#include <memory>
#include <gtest/gtest.h>
#include <fstream>

#include "OpticsParser.hpp"

class TestDifferentEmissivities : public testing::Test
{
protected:
    void SetUp() override
    {
        std::string inputContent = "{ Units, Wavelength Units } SI Microns\n"
                                   "{ Thickness } 3.048 \n"
                                   "{ Conductivity } 1\n"
                                   "{ IR Transmittance } TIR=0\n"
                                   "{ Emissivity, front back } Emis= 0.5 0.84\n"
                                   "{ }\n"
                                   "{ Ef_Source: Material }\n"
                                   "{ Eb_Source: Material }\n"
                                   "{ IGDB_Checksum: -1717699038 }\n"
                                   "{ Product Name: Generic Clear Glass }\n"
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

        std::ofstream out("InputDifferentEmissivities.dat");
        out << inputContent;
        out.close();
    }
};

TEST_F(TestDifferentEmissivities, Test1)
{
    const std::string inputFile = R"(InputDifferentEmissivities.dat)";
    OpticsParser::Parser parser;
    OpticsParser::ProductData product = parser.parseFile(inputFile);

    EXPECT_NEAR(3.048, product.thickness, 1e-6);
    EXPECT_NEAR(1, product.conductivity.value(), 1e-6);
    EXPECT_NEAR(0, product.IRTransmittance, 1e-6);
    EXPECT_NEAR(0.5, product.frontEmissivity, 1e-6);
    EXPECT_NEAR(0.84, product.backEmissivity, 1e-6);
    EXPECT_EQ(102, product.nfrcid);
    EXPECT_EQ("Generic Clear Glass", product.productName);
    EXPECT_EQ("Monolithic", product.productType);

    std::vector<OpticsParser::WLData> correctResults{{0.300, 0.0020, 0.0470, 0.0480},
                                                     {0.305, 0.0030, 0.0470, 0.0480},
                                                     {0.310, 0.0090, 0.0470, 0.0480},
                                                     {0.315, 0.0350, 0.0470, 0.0480},
                                                     {0.320, 0.1000, 0.0470, 0.0480},
                                                     {0.325, 0.2180, 0.0490, 0.0500}};

    EXPECT_EQ(correctResults.size(), product.measurements.size());
    for(auto i = 0u; i < correctResults.size(); ++i)
    {
        EXPECT_NEAR(correctResults[i].wavelength, product.measurements[i].wavelength, 1e-6);
        EXPECT_NEAR(
          correctResults[i].directComponent.tf, product.measurements[i].directComponent.tf, 1e-6);
        EXPECT_NEAR(
          correctResults[i].directComponent.rf, product.measurements[i].directComponent.rf, 1e-6);
        EXPECT_NEAR(
          correctResults[i].directComponent.rb, product.measurements[i].directComponent.rb, 1e-6);
    }
}
