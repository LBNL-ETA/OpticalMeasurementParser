#include <memory>
#include <gtest/gtest.h>
#include <memory>
#include <sstream>
//#include <filesystem>

#include "Parser.hpp"

#include "paths.h"

extern std::string test_dir;


class TestLoadOpticsFileFromDisk : public testing::Test
{
protected:
    virtual void SetUp()
    {}
};


TEST_F(TestLoadOpticsFileFromDisk, TestLoadClear3)
{
    SCOPED_TRACE("Begin Test: Load CLEAR_3.DAT from disk.");

    std::string clear_3_path(test_dir);
    clear_3_path += "/products";
    clear_3_path += "/CLEAR_3.DAT";

    OpticsParser::Parser parser;
    OpticsParser::ProductData product = parser.parseFile(clear_3_path);
    EXPECT_EQ(product.nfrcid, 102);
    EXPECT_EQ(product.productName, "Generic Clear Glass");
    EXPECT_EQ(product.productType, "Monolithic");
    EXPECT_EQ(product.thickness, 3.048);
    EXPECT_EQ(product.conductivity, 1.0);
    EXPECT_EQ(product.IRTransmittance, 0.0);
    EXPECT_EQ(product.frontEmissivity, 0.84);
    EXPECT_EQ(product.backEmissivity, 0.84);
    EXPECT_EQ(product.measurements.size(), 111);
    EXPECT_EQ(product.measurements[0].wavelength, 0.3);
    EXPECT_EQ(product.measurements[0].T, 0.002);
    EXPECT_EQ(product.measurements[0].frontR, 0.047);
    EXPECT_EQ(product.measurements[0].backR, 0.048);
    EXPECT_EQ(product.measurements[110].wavelength, 2.5);
    EXPECT_EQ(product.measurements[110].T, 0.822);
    EXPECT_EQ(product.measurements[110].frontR, 0.068);
    EXPECT_EQ(product.measurements[110].backR, 0.068);
    EXPECT_EQ(product.frontEmissivitySource, "Material");
    EXPECT_EQ(product.backEmissivitySource, "Material");
    EXPECT_EQ(product.manufacturer, "Generic");
    EXPECT_EQ(product.material, "Glass");
    EXPECT_EQ(product.coatingName, "N/A");
    EXPECT_EQ(product.coatedSide, "Neither");
    EXPECT_EQ(product.substrateFilename, "N/A");
    EXPECT_EQ(product.appearance, "Clear");
    EXPECT_EQ(product.acceptance, "#");
    EXPECT_EQ(product.unitSystem, "SI");
    EXPECT_EQ(product.wavelengthUnit, "Microns");
}
