#pragma once

#include <optional>
#include <gtest/gtest.h>

namespace Helper
{
    template <typename T>
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
    template <>
    void compareOptional(const std::optional<double>& expected, const std::optional<double>& actual)
    {
        if (expected.has_value())
        {
            ASSERT_TRUE(actual.has_value());
            EXPECT_DOUBLE_EQ(expected.value(), actual.value());
        }
        else
        {
            EXPECT_FALSE(actual.has_value());
        }
    }
}