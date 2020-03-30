#include <memory>
#include <gtest/gtest.h>
#include <memory>
#include <sstream>
#include <filesystem>

#include "Parser.hpp"

#include "paths.h"

extern std::string test_dir;

#if 0
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
//    EXPECT_EQ(product->nfrcid, 102);
    EXPECT_EQ(product->productName, "Generic Clear Glass");
    EXPECT_EQ(product->productType, "monolithic");
    EXPECT_EQ(product->thickness, 3.048);
    EXPECT_EQ(product->conductivity, 1.0);
    EXPECT_EQ(product->IRTransmittance, 0.0);
    EXPECT_EQ(product->frontEmissivity, 0.84);
    EXPECT_EQ(product->backEmissivity, 0.84);
    EXPECT_EQ(product->measurements.size(), 111);
    EXPECT_EQ(product->measurements[0].wavelength, 0.3);
    EXPECT_EQ(product->measurements[0].T, 0.002);
    EXPECT_EQ(product->measurements[0].frontR, 0.047);
    EXPECT_EQ(product->measurements[0].backR, 0.048);
    EXPECT_EQ(product->measurements[110].wavelength, 2.5);
    EXPECT_EQ(product->measurements[110].T, 0.822);
    EXPECT_EQ(product->measurements[110].frontR, 0.068);
    EXPECT_EQ(product->measurements[110].backR, 0.068);
}
#endif
