// Copyright 2021, 2022 by Mark Melton
//

#include <gtest/gtest.h>
#include "core/fp/seq.h"

using namespace core::fp::detail;

TEST(FpSeq, Sequence)
{
    EXPECT_FALSE(Sequence<int>);
    EXPECT_TRUE(Sequence<std::vector<int>>);
}

int main(int argc, char *argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
