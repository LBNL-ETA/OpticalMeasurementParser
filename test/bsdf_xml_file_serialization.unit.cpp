#include <memory>
#include <gtest/gtest.h>
#include <memory>
#include <sstream>
#include <filesystem>

#include "BSDFXML/Parser.hpp"

#include "paths.h"
#include "test/helper/BSDFXML/TestHelper.hpp"

TEST(BSDFXMLFileSerialization, Load2011SA1SmallMaterial)
{
    SCOPED_TRACE("Begin Test: Load 2011-SA1-Small.XML");
    std::filesystem::path product_path(test_dir);
    product_path /= "products";
    product_path /= "2011-SA1-Small.XML";

    auto product = BSDFXML::loadWindowElementFromFile(product_path.string());

    ASSERT_TRUE(product.has_value());
    EXPECT_EQ(BSDFXML::WindowElementType::System, product->windowElementType);
    ASSERT_TRUE(product->optical.layer.material.has_value());

    // Define the expected material for comparison
    BSDFXML::Material expectedMaterial;
    expectedMaterial.name = "Satine 5500 5%, White Pearl";
    expectedMaterial.manufacturer = "Nysan";
    expectedMaterial.thickness = BSDFXML::Thickness{1.0, BSDFXML::LengthUnit::Millimeter};
    expectedMaterial.deviceType = BSDFXML::DeviceType::Other;
    expectedMaterial.thermalConductivity = 0.15;
    expectedMaterial.airPermeability = std::nullopt;
    expectedMaterial.emissivityFront = 0.79626;
    expectedMaterial.emissivityBack = 0.79626;
    expectedMaterial.TIR = 0.10916;
    expectedMaterial.effectiveOpennessFraction = std::nullopt;
    expectedMaterial.permeabilityFactor = 0.049855;
    expectedMaterial.AERCAcceptance = "@";

    Helper::compareMaterial(expectedMaterial, product->optical.layer.material.value());
}

TEST(BSDFXMLFileSerialization, Load2011SA1SmallDataDefintion)
{
    SCOPED_TRACE("Begin Test: Load 2011-SA1-Small.XML");
    std::filesystem::path product_path(test_dir);
    product_path /= "products";
    product_path /= "2011-SA1-Small.XML";

    auto product = BSDFXML::loadWindowElementFromFile(product_path.string());

    ASSERT_TRUE(product.has_value());
    EXPECT_EQ(BSDFXML::WindowElementType::System, product->windowElementType);
    ASSERT_TRUE(product->optical.layer.dataDefinition.has_value());

    BSDFXML::DataDefinition expectedDataDefinition;
    expectedDataDefinition.incidentDataStructure = BSDFXML::IncidentDataStructure::Columns;

    // Set expected values for AngleBasis
    BSDFXML::AngleBasis angleBasis;
    angleBasis.name = "LBNL/Klems Full";

    // Fill expected blocks for AngleBasis
    angleBasis.blocks = {
        {0, std::nullopt, 1, BSDFXML::ThetaBounds{0, 5, std::nullopt}, std::nullopt},
        {10, std::nullopt, 8, BSDFXML::ThetaBounds{5, 15, std::nullopt}, std::nullopt},
        {20, std::nullopt, 16, BSDFXML::ThetaBounds{15, 25, std::nullopt}, std::nullopt},
        {30, std::nullopt, 20, BSDFXML::ThetaBounds{25, 35, std::nullopt}, std::nullopt},
        {40, std::nullopt, 24, BSDFXML::ThetaBounds{35, 45, std::nullopt}, std::nullopt},
        {50, std::nullopt, 24, BSDFXML::ThetaBounds{45, 55, std::nullopt}, std::nullopt},
        {60, std::nullopt, 24, BSDFXML::ThetaBounds{55, 65, std::nullopt}, std::nullopt},
        {70, std::nullopt, 16, BSDFXML::ThetaBounds{65, 75, std::nullopt}, std::nullopt},
        {82.5, std::nullopt, 12, BSDFXML::ThetaBounds{75, 90, std::nullopt}, std::nullopt}
    };

    expectedDataDefinition.angleBasis = angleBasis;

    Helper::compareDataDefinition(expectedDataDefinition, product->optical.layer.dataDefinition.value());
}