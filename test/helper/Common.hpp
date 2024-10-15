#pragma once

#include <optional>
#include <gtest/gtest.h>

namespace Helper
{
    template<typename T>
    void compareOptional(const std::optional<T> & expected, const std::optional<T> & actual)
    {
        if(expected.has_value())
        {
            ASSERT_TRUE(actual.has_value());
            EXPECT_EQ(expected.value(), actual.value());
        }
        else
        {
            EXPECT_FALSE(actual.has_value());
        }
    }

    // Specialization for comparing optional double values
    template<>
    void compareOptional(const std::optional<double> & expected,
                         const std::optional<double> & actual)
    {
        if(expected.has_value())
        {
            ASSERT_TRUE(actual.has_value());
            EXPECT_DOUBLE_EQ(expected.value(), actual.value());
        }
        else
        {
            EXPECT_FALSE(actual.has_value());
        }
    }

    template<typename T>
    void compareVector(const std::vector<T> & expected, const std::vector<T> & actual)
    {
        ASSERT_EQ(expected.size(), actual.size());
        for(size_t i = 0; i < expected.size(); ++i)
        {
            EXPECT_EQ(expected[i], actual[i]);
        }
    }

    template<>
    void compareVector(const std::vector<double> & expected, const std::vector<double> & actual)
    {
        ASSERT_EQ(expected.size(), actual.size());
        for(size_t i = 0; i < expected.size(); ++i)
        {
            EXPECT_DOUBLE_EQ(expected[i], actual[i]);
        }
    }

    void compareVector(const std::vector<double> & expected,
                       const std::vector<double> & actual,
                       const double tolerance = 1e-6)
    {
        ASSERT_EQ(expected.size(), actual.size());
        for(size_t i = 0; i < expected.size(); ++i)
        {
            EXPECT_NEAR(expected[i], actual[i], tolerance);
        }
    }
}   // namespace Helper