#include <gtest/gtest.h>

#include "TestHelper.hpp"
#include "test/helper/Common.hpp"

namespace Helper
{
    void compareThickness(const BSDFXML::Thickness & expected, const BSDFXML::Thickness & actual)
    {
        EXPECT_DOUBLE_EQ(expected.value, actual.value);
        EXPECT_EQ(expected.unit, actual.unit);
    }

    void compareThickness(const std::optional<BSDFXML::Thickness> & expected,
                          const std::optional<BSDFXML::Thickness> & actual)
    {
        if(expected.has_value())
        {
            ASSERT_TRUE(actual.has_value());
            compareThickness(expected.value(), actual.value());
        }
        else
        {
            EXPECT_FALSE(actual.has_value());
        }
    }

    void compareWavelength(const BSDFXML::Wavelength & expected, const BSDFXML::Wavelength & actual)
    {
        EXPECT_EQ(expected.value, actual.value);
        EXPECT_EQ(expected.unit, actual.unit);
        compareOptional(expected.type, actual.type);
    }

    void compareWavelength(const std::optional<BSDFXML::Wavelength> & expected,
                                   const std::optional<BSDFXML::Wavelength> & actual)
    {
        if(expected.has_value())
        {
            ASSERT_TRUE(actual.has_value());
            compareWavelength(expected.value(), actual.value());
        }
        else
        {
            EXPECT_FALSE(actual.has_value());
        }
    }

    void compareMaterial(const BSDFXML::Material & expected, const BSDFXML::Material & actual)
    {
        EXPECT_EQ(expected.name, actual.name);

        compareOptional(expected.productName, actual.productName);
        compareOptional(expected.manufacturer, actual.manufacturer);
        compareOptional(expected.manufacturer, actual.manufacturer);
        compareThickness(expected.thickness, actual.thickness);
        compareOptional(actual.deviceType, expected.deviceType);
        compareOptional(actual.thermalConductivity, expected.thermalConductivity);
        compareOptional(actual.airPermeability, expected.airPermeability);
        compareOptional(actual.emissivityFront, expected.emissivityFront);
        compareOptional(actual.emissivityBack, expected.emissivityBack);
        compareOptional(actual.TIR, expected.TIR);
        compareOptional(actual.effectiveOpennessFraction, expected.effectiveOpennessFraction);
        compareOptional(actual.permeabilityFactor, expected.permeabilityFactor);
        // compareOptional(actual.opticalProperties, expected.opticalProperties);
        compareOptional(actual.color, expected.color);
        compareOptional(actual.AERCAcceptance, expected.AERCAcceptance);
        compareOptional(actual.comments, expected.comments);
        compareOptional(actual.width, expected.width);
        compareOptional(actual.height, expected.height);
    }

    void compareThetaBounds(const BSDFXML::ThetaBounds & expected,
                            const BSDFXML::ThetaBounds & actual)
    {
        compareOptional(expected.lowerTheta, actual.lowerTheta);
        compareOptional(expected.upperTheta, actual.upperTheta);
        compareOptional(expected.comments, actual.comments);
    }

    void compareBounds(const std::optional<std::variant<double, BSDFXML::ThetaBounds>> & expected,
                       const std::optional<std::variant<double, BSDFXML::ThetaBounds>> & actual)
    {
        if(expected.has_value())
        {
            ASSERT_TRUE(actual.has_value());
            if(std::holds_alternative<double>(expected.value()))
            {
                ASSERT_TRUE(std::holds_alternative<double>(actual.value()));
                EXPECT_DOUBLE_EQ(std::get<double>(expected.value()), std::get<double>(actual.value()));
            }
            else
            {
                ASSERT_TRUE(std::holds_alternative<BSDFXML::ThetaBounds>(actual.value()));
                compareThetaBounds(std::get<BSDFXML::ThetaBounds>(expected.value()),
                                   std::get<BSDFXML::ThetaBounds>(actual.value()));
            }
        }
        else
        {
            EXPECT_FALSE(actual.has_value());
        }
    }

    void compareAngleBasisBlock(const BSDFXML::AngleBasisBlock & expected,
                                const BSDFXML::AngleBasisBlock & actual)
    {
        EXPECT_DOUBLE_EQ(expected.theta, actual.theta);
        compareOptional(expected.phi, actual.phi);
        compareOptional(expected.nPhis, actual.nPhis);
        compareBounds(expected.bounds, actual.bounds);
        compareOptional(expected.comments, actual.comments);
    }

    void compareAngleBasis(const BSDFXML::AngleBasis & expected, const BSDFXML::AngleBasis & actual)
    {
        EXPECT_EQ(expected.name, actual.name);
        ASSERT_EQ(expected.blocks.size(), actual.blocks.size());
        for(size_t i = 0; i < expected.blocks.size(); ++i)
        {
            compareAngleBasisBlock(expected.blocks[i], actual.blocks[i]);
        }
    }

    void compareDataDefinition(const BSDFXML::DataDefinition & expected,
                               const BSDFXML::DataDefinition & actual)
    {
        EXPECT_EQ(expected.incidentDataStructure, actual.incidentDataStructure);
        compareAngleBasis(expected.angleBasis, actual.angleBasis);
    }

    void compareScatteringData(const BSDFXML::ScatteringData & expected,
                               const BSDFXML::ScatteringData & actual)
    {
        ASSERT_EQ(expected.size(), actual.size());
        for(size_t i = 0; i < expected.size(); ++i)
        {
            compareVector(expected[i], actual[i], 1e-5); // saved only up to 5 decimal places
        }
    }

    void compareWavelengthDataBlock(const BSDFXML::WavelengthDataBlock & expected,
                                    const BSDFXML::WavelengthDataBlock & actual)
    {
        compareOptional(expected.wavelengthDataDirection, actual.wavelengthDataDirection);
        compareOptional(expected.columnAngleBasis, actual.columnAngleBasis);
        compareOptional(expected.rowAngleBasis, actual.rowAngleBasis);
        compareOptional(expected.scatteringDataType, actual.scatteringDataType);
        compareScatteringData(expected.scatteringData, actual.scatteringData);
    }

    void compareWavelengthData(const BSDFXML::WavelengthData & expected,
                               const BSDFXML::WavelengthData & actual)
    {
        compareOptional(expected.layerNumber, actual.layerNumber);
        compareOptional(expected.angle, actual.angle);
        compareWavelength(expected.wavelength, actual.wavelength);
        compareOptional(expected.sourceSpectrum, actual.sourceSpectrum);
        compareOptional(expected.detectorSpectrum, actual.detectorSpectrum);
        compareWavelengthDataBlock(expected.block, actual.block);

        compareOptional(expected.comments, actual.comments);
    }
}   // namespace Helper