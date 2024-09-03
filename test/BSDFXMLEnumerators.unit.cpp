#include <gtest/gtest.h>

#include "BSDFXML/Enumerators.hpp"

using namespace BSDFXML;

TEST(EnumConversionTest, WindowElementTypeToString)
{
    EXPECT_EQ(windowElementToString(WindowElementType::Unknown), "Unknown");
    EXPECT_EQ(windowElementToString(WindowElementType::System), "System");
    EXPECT_EQ(windowElementToString(WindowElementType::Layer), "Layer");
    EXPECT_EQ(windowElementToString(WindowElementType::Material), "Material");
}

TEST(EnumConversionTest, WindowElementFromString)
{
    EXPECT_EQ(windowElementFromString("Unknown"), WindowElementType::Unknown);
    EXPECT_EQ(windowElementFromString("System"), WindowElementType::System);
    EXPECT_EQ(windowElementFromString("Layer"), WindowElementType::Layer);
    EXPECT_EQ(windowElementFromString("Material"), WindowElementType::Material);
    EXPECT_EQ(windowElementFromString("NonExistentValue"),
              WindowElementType::Unknown);   // Test for default case
}

TEST(EnumConversionTest, DeviceTypeToString)
{
    EXPECT_EQ(DeviceTypeToString(DeviceType::Unknown), "Unknown");
    EXPECT_EQ(DeviceTypeToString(DeviceType::VenetianBlindHorizontal), "Venetian blind horizontal");
    EXPECT_EQ(DeviceTypeToString(DeviceType::VenetianBlindVertical), "Venetian blind vertical");
    EXPECT_EQ(DeviceTypeToString(DeviceType::WovenShade), "Woven shade");
    EXPECT_EQ(DeviceTypeToString(DeviceType::DiffusingGlass), "Diffusing glass");
    EXPECT_EQ(DeviceTypeToString(DeviceType::SpecularGlass), "Specular glass");
    EXPECT_EQ(DeviceTypeToString(DeviceType::FrittedGlass), "Fritted Glass");
    EXPECT_EQ(DeviceTypeToString(DeviceType::CellularShade), "Cellular Shade");
    EXPECT_EQ(DeviceTypeToString(DeviceType::PleatedShade), "Pleated Shade");
    EXPECT_EQ(DeviceTypeToString(DeviceType::SolidLayer), "Solid Layer");
    EXPECT_EQ(DeviceTypeToString(DeviceType::Other), "Other");
}

TEST(EnumConversionTest, DeviceTypeFromString)
{
    EXPECT_EQ(DeviceTypeFromString("Unknown"), DeviceType::Unknown);
    EXPECT_EQ(DeviceTypeFromString("Venetian blind horizontal"),
              DeviceType::VenetianBlindHorizontal);
    EXPECT_EQ(DeviceTypeFromString("Venetian blind vertical"), DeviceType::VenetianBlindVertical);
    EXPECT_EQ(DeviceTypeFromString("Woven shade"), DeviceType::WovenShade);
    EXPECT_EQ(DeviceTypeFromString("Diffusing glass"), DeviceType::DiffusingGlass);
    EXPECT_EQ(DeviceTypeFromString("Specular glass"), DeviceType::SpecularGlass);
    EXPECT_EQ(DeviceTypeFromString("Fritted Glass"), DeviceType::FrittedGlass);
    EXPECT_EQ(DeviceTypeFromString("Cellular Shade"), DeviceType::CellularShade);
    EXPECT_EQ(DeviceTypeFromString("Pleated Shade"), DeviceType::PleatedShade);
    EXPECT_EQ(DeviceTypeFromString("Solid Layer"), DeviceType::SolidLayer);
    EXPECT_EQ(DeviceTypeFromString("Other"), DeviceType::Other);
    EXPECT_EQ(DeviceTypeFromString("NonExistentValue"),
              DeviceType::Unknown);   // Test for default case
}

TEST(EnumConversionTest, IncidentDataStructureToString)
{
    EXPECT_EQ(IncidentDataStructureToString(IncidentDataStructure::Unknown), "Unknown");
    EXPECT_EQ(IncidentDataStructureToString(IncidentDataStructure::Columns), "Columns");
}

TEST(EnumConversionTest, IncidentDataStructureFromString)
{
    EXPECT_EQ(IncidentDataStructureFromString("Unknown"), IncidentDataStructure::Unknown);
    EXPECT_EQ(IncidentDataStructureFromString("Columns"), IncidentDataStructure::Columns);
    EXPECT_EQ(IncidentDataStructureFromString("NonExistentValue"),
              IncidentDataStructure::Unknown);   // Test for default case
}

TEST(EnumConversionTest, WavelengthDataDirectionToString)
{
    EXPECT_EQ(WavelengthDataDirectionToString(WavelengthDataDirection::Unknown), "Unknown");
    EXPECT_EQ(WavelengthDataDirectionToString(WavelengthDataDirection::TransmissionFront),
              "Transmission Front");
    EXPECT_EQ(WavelengthDataDirectionToString(WavelengthDataDirection::TransmissionBack),
              "Transmission Back");
    EXPECT_EQ(WavelengthDataDirectionToString(WavelengthDataDirection::ReflectionFront),
              "Reflection Front");
    EXPECT_EQ(WavelengthDataDirectionToString(WavelengthDataDirection::ReflectionBack),
              "Reflection Back");
    EXPECT_EQ(WavelengthDataDirectionToString(WavelengthDataDirection::AbsorbedFront),
              "Absorbed Front");
    EXPECT_EQ(WavelengthDataDirectionToString(WavelengthDataDirection::AbsorbedBack),
              "Absorbed Back");
}

TEST(EnumConversionTest, WavelengthDataDirectionFromString)
{
    EXPECT_EQ(WavelengthDataDirectionFromString("Unknown"), WavelengthDataDirection::Unknown);
    EXPECT_EQ(WavelengthDataDirectionFromString("Transmission Front"),
              WavelengthDataDirection::TransmissionFront);
    EXPECT_EQ(WavelengthDataDirectionFromString("Transmission Back"),
              WavelengthDataDirection::TransmissionBack);
    EXPECT_EQ(WavelengthDataDirectionFromString("Reflection Front"),
              WavelengthDataDirection::ReflectionFront);
    EXPECT_EQ(WavelengthDataDirectionFromString("Reflection Back"),
              WavelengthDataDirection::ReflectionBack);
    EXPECT_EQ(WavelengthDataDirectionFromString("Absorbed Front"),
              WavelengthDataDirection::AbsorbedFront);
    EXPECT_EQ(WavelengthDataDirectionFromString("Absorbed Back"),
              WavelengthDataDirection::AbsorbedBack);
    EXPECT_EQ(WavelengthDataDirectionFromString("NonExistentValue"),
              WavelengthDataDirection::Unknown);   // Test for default case
}

TEST(EnumConversionTest, ScatteringDataTypeToString)
{
    EXPECT_EQ(ScatteringDataTypeToString(ScatteringDataType::Unknown), "Unknown");
    EXPECT_EQ(ScatteringDataTypeToString(ScatteringDataType::BTDF), "BTDF");
    EXPECT_EQ(ScatteringDataTypeToString(ScatteringDataType::FractionalDiffuse), "Fractional Diffuse");
    EXPECT_EQ(ScatteringDataTypeToString(ScatteringDataType::FractionalSpecular), "Fractional Specular");
}

TEST(EnumConversionTest, ScatteringDataTypeFromString)
{
    EXPECT_EQ(ScatteringDataTypeFromString("Unknown"), ScatteringDataType::Unknown);
    EXPECT_EQ(ScatteringDataTypeFromString("BTDF"), ScatteringDataType::BTDF);
    EXPECT_EQ(ScatteringDataTypeFromString("Fractional Diffuse"), ScatteringDataType::FractionalDiffuse);
    EXPECT_EQ(ScatteringDataTypeFromString("Fractional Specular"), ScatteringDataType::FractionalSpecular);
    EXPECT_EQ(ScatteringDataTypeFromString("NonExistentValue"),
              ScatteringDataType::Unknown);   // Test for default case
}