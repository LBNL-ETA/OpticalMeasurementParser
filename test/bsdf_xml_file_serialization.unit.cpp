#include <memory>
#include <gtest/gtest.h>
#include <memory>
#include <sstream>
#include <filesystem>

#include "BSDFXML/Parser.hpp"

#include "paths.h"

//extern std::string test_dir;

TEST(BSDFXMLFileSerialization, Load2011SA1Small)
{
    SCOPED_TRACE("Begin Test: Load 2011-SA1-Small.XML");
    std::filesystem::path product_path(test_dir);
    product_path /= "products";
    product_path /= "2011-SA1-Small.XML";

    auto product = BSDFXML::loadWindowElementFromFile(product_path.string());

    ASSERT_TRUE(product.has_value());
    EXPECT_EQ(BSDFXML::WindowElementType::System, product->windowElementType);
}
