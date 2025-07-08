#pragma once

#include "..\lib\ts\suite.hpp"

ts::Suite tests { "Test ts library" };

TEST(tests, "Multiple failing asserts.")
{
    ASSERT_EQ(7, 8)
    ASSERT_TRUE(false)
    ASSERT_FALSE(true)
    ASSERT_NEQ(7, 7)
}

TEST(tests, "Multiple passing asserts.")
{
    ASSERT_EQ(7, 7)
    ASSERT_TRUE(true)
    ASSERT_FALSE(false)
    ASSERT_NEQ(7, 8)
}

class Data
{
public:
    int x;
    bool operator==(const Data& other) const { return this->x == other.x; }
};

TEST(tests, "Failing tests of non-streamable class.")
{
    Data d_0 { 7 };
    Data d_1 { 8 };
    Data d_2 { 7 };
    ASSERT_EQ(d_0, d_1)
    ASSERT_NEQ(d_0, d_2)
}

#pragma once

#include <iostream>

#include "../lib/ts/suite.hpp"


ts::Suite some { "Now this is something." };

TEST(some, "Oh yeah.")
{
    ASSERT_TRUE(true)
}

