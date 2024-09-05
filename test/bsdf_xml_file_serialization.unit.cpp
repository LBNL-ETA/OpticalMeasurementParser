#include <memory>
#include <gtest/gtest.h>
#include <memory>
#include <sstream>
#include <filesystem>

#include "BSDFXML/Parser.hpp"

#include "paths.h"

// extern std::string test_dir;

TEST(BSDFXMLFileSerialization, Load2011SA1Small)
{
    SCOPED_TRACE("Begin Test: Load 2011-SA1-Small.XML");
    std::filesystem::path product_path(test_dir);
    product_path /= "products";
    product_path /= "2011-SA1-Small.XML";

    auto product = BSDFXML::loadWindowElementFromFile(product_path.string());

    ASSERT_TRUE(product.has_value());
    EXPECT_EQ(BSDFXML::WindowElementType::System, product->windowElementType);
    ASSERT_TRUE(product->optical.layer.material.has_value());
    auto & mat{product->optical.layer.material.value()};
    EXPECT_EQ("Satine 5500 5%, White Pearl", mat.name);
    EXPECT_EQ("Nysan", mat.manufacturer);

    ASSERT_TRUE(mat.thickness.has_value());
    EXPECT_DOUBLE_EQ(1.0, mat.thickness->value);
    EXPECT_EQ(BSDFXML::LengthUnit::Millimeter, mat.thickness->unit);

    ASSERT_TRUE(mat.deviceType.has_value());
    EXPECT_EQ(BSDFXML::DeviceType::Other, mat.deviceType.value());

    ASSERT_TRUE(mat.thermalConductivity.has_value());
    EXPECT_DOUBLE_EQ(0.15, mat.thermalConductivity.value());

    EXPECT_TRUE(!mat.airPermeability.has_value());

    ASSERT_TRUE(mat.emissivityFront.has_value());
    EXPECT_DOUBLE_EQ(0.79626, mat.emissivityFront.value());

    ASSERT_TRUE(mat.emissivityBack.has_value());
    EXPECT_DOUBLE_EQ(0.79626, mat.emissivityBack.value());

    ASSERT_TRUE(mat.TIR.has_value());
    EXPECT_DOUBLE_EQ(0.10916, mat.TIR.value());

    EXPECT_TRUE(!mat.effectiveOpennessFraction.has_value());

    ASSERT_TRUE(mat.permeabilityFactor.has_value());
    EXPECT_DOUBLE_EQ(0.049855, mat.permeabilityFactor.value());

    ASSERT_TRUE(!mat.opticalProperties.has_value());
    ASSERT_TRUE(!mat.color.has_value());

    ASSERT_TRUE(mat.AERCAcceptance.has_value());
    EXPECT_EQ("@", mat.AERCAcceptance.value());

    ASSERT_TRUE(!mat.comments.has_value());
    ASSERT_TRUE(!mat.width.has_value());
    ASSERT_TRUE(!mat.height.has_value());
    ASSERT_TRUE(!mat.openness.has_value());
}
