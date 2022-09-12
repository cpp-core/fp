// Copyright (C) 2019, 2021, 2022 by Mark Melton
//

#include <gtest/gtest.h>
#include "core/util/common.h"
#include "core/fp/fold.h"

using namespace core::fp;

TEST(Fp, FoldLeftEmpty)
{
    strings strs = { };
    EXPECT_THROW(fold_l(strs, [](auto a, auto b) { return a + ":" + b; }),
		 std::invalid_argument);
}


TEST(Fp, FoldLeftSingle)
{
    strings strs = { "abc" }; 
    auto r = fold_l(strs, [](auto a, auto b) { return a + ":"s + b; });
    EXPECT_EQ(r, "abc");
}

TEST(Fp, FoldLeftMultipleStrings)
{
    strings strs = { "abc", "def", "ghi" };
    auto r = fold_l(strs, [](auto a, auto b) { return a + ":"s + b; });
    EXPECT_EQ(r, "abc:def:ghi");
}

TEST(Fp, FoldLeftMultipleInts)
{
    ints values = { 1, 2, 3 };
    {
	auto r = fold_l(values, [](auto a, auto b) { return a + b; });
	EXPECT_EQ(r, 6);
    }
    {
	auto r = fold_l(values, 1, [](auto a, auto b) { return a + b; });
	EXPECT_EQ(r, 7);
    }
}

TEST(Fp, FoldAny)
{
    auto r0 = any(vector<bool>{}, [](const auto& elem) { return elem; });
    EXPECT_EQ(false, r0);
    
    auto r1 = any(vector<bool>{false}, [](const auto& elem) { return elem; });
    EXPECT_EQ(false, r1);
    
    auto r2 = any(vector<bool>{true}, [](const auto& elem) { return elem; });
    EXPECT_EQ(true, r2);
    
    auto r3 = any(vector<bool>{false, true}, [](const auto& elem) { return elem; });
    EXPECT_EQ(true, r3);
    
    auto r4 = any(vector<bool>{true, true}, [](const auto& elem) { return elem; });
    EXPECT_EQ(true, r4);
    
    auto r5 = any(vector<bool>{false, false}, [](const auto& elem) { return elem; });
    EXPECT_EQ(false, r5);
}

TEST(Fp, FoldAll)
{
    auto r0 = all(vector<bool>{}, [](const auto& elem) { return elem; });
    EXPECT_EQ(true, r0);
    
    auto r1 = all(vector<bool>{false}, [](const auto& elem) { return elem; });
    EXPECT_EQ(false, r1);
    
    auto r2 = all(vector<bool>{true}, [](const auto& elem) { return elem; });
    EXPECT_EQ(true, r2);
    
    auto r3 = all(vector<bool>{false, true}, [](const auto& elem) { return elem; });
    EXPECT_EQ(false, r3);
    
    auto r4 = all(vector<bool>{true, true}, [](const auto& elem) { return elem; });
    EXPECT_EQ(true, r4);

    auto r5 = all(vector<bool>{false, false}, [](const auto& elem) { return elem; });
    EXPECT_EQ(false, r5);
}

int main(int argc, char *argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
