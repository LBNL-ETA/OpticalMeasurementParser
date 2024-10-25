#include <memory>
#include <gtest/gtest.h>
#include <sstream>
#include <filesystem>

#include "Parser.hpp"


TEST(TestInvalidXML, TestLoad2011SA1)
{
    SCOPED_TRACE("Begin Test: invalid.XML");
    std::filesystem::path product_path(TEST_DATA_DIR);
    product_path /= "products";
    product_path /= "invalid.xml";

    EXPECT_THROW(OpticsParser::parseBSDFXMLFile(product_path.string()), std::runtime_error);
}
