#include <memory>
#include <gtest/gtest.h>
#include <fstream>

#include "OpticsParser.hpp"

class TestFile1 : public testing::Test
{
protected:
    void SetUp() override
    {
        std::string inputContent = "{ Units, Wavelength Units } SI Microns\n"
                                   "{ Thickness } 3.048 \n"
                                   "{ Conductivity } 1\n"
                                   "{ IR Transmittance } TIR=0\n"
                                   "{ Emissivity, front back } Emis= 0.84 0.84\n"
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

        std::ofstream out("InputFile1.dat");
        out << inputContent;
        out.close();
    }
};

TEST_F(TestFile1, Test1)
{
    const std::string inputFile = R"(InputFile1.dat)";
    OpticsParser::Parser par(inputFile);

    const auto thickness = par.thickness();
    EXPECT_NEAR(3.048, thickness, 1e-6);

    const auto conductivity = par.conductivity();
    EXPECT_NEAR(1, conductivity, 1e-6);

    const auto irTransmittance = par.IRTransmittance();
    EXPECT_NEAR(0, irTransmittance, 1e-6);

    const auto frontEmissivity = par.frontEmissivity();
    EXPECT_NEAR(0.84, frontEmissivity, 1e-6);

    const auto backEmissivity = par.backEmissivity();
    EXPECT_NEAR(0.84, backEmissivity, 1e-6);

    const auto NFRCID = par.nfrcid();
    EXPECT_EQ(102, NFRCID);

    const auto & productName = par.productName();
    EXPECT_EQ("Generic Clear Glass", productName);

    const auto measurements = par.measurements();
    std::vector<OpticsParser::WLData> correctResults{{0.300, 0.0020, 0.0470, 0.0480},
                                                     {0.305, 0.0030, 0.0470, 0.0480},
                                                     {0.310, 0.0090, 0.0470, 0.0480},
                                                     {0.315, 0.0350, 0.0470, 0.0480},
                                                     {0.320, 0.1000, 0.0470, 0.0480},
                                                     {0.325, 0.2180, 0.0490, 0.0500}};

    EXPECT_EQ(correctResults.size(), measurements.size());
    for(auto i = 0u; i < correctResults.size(); ++i)
    {
        EXPECT_NEAR(correctResults[i].wavelength, measurements[i].wavelength, 1e-6);
        EXPECT_NEAR(correctResults[i].T, measurements[i].T, 1e-6);
        EXPECT_NEAR(correctResults[i].frontR, measurements[i].frontR, 1e-6);
        EXPECT_NEAR(correctResults[i].backR, measurements[i].backR, 1e-6);
    }
}
