#include <memory>
#include <gtest/gtest.h>
#include <memory>
#include <sstream>
#include <filesystem>

#include "Parser.hpp"

#include "paths.h"

extern std::string test_dir;

class TestInvalidXML : public testing::Test
{
protected:
    virtual void SetUp()
    {}
};


TEST_F(TestInvalidXML, TestLoad2011SA1)
{
    SCOPED_TRACE("Begin Test: invalid.XML");
    std::filesystem::path product_path(test_dir);
    product_path /= "products";
    product_path /= "invalid.xml";

    EXPECT_THROW(OpticsParser::parseBSDFXMLFile(product_path.string()), std::runtime_error);
}
