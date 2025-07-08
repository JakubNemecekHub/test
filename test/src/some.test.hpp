#pragma once
#include "../lib/ts/suite.hpp"


ts::Suite some { "Now this is something." };

TEST(some, "Oh yeah.")
{
    ASSERT_FALSE(false)
}
