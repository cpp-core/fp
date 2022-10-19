// Copyright (C) 2019, 2021, 2022 by Mark Melton
//

#include <gtest/gtest.h>
#include "core/fp/fold.h"

using namespace core::fp;
using namespace std::literals::string_literals;

TEST(FpFold, LeftEmpty)
{
    std::vector<std::string> strs = { };
    EXPECT_THROW(fold_l(strs, [](auto a, auto b) { return a + ":" + b; }),
		 std::invalid_argument);
}


TEST(FpFold, LeftSingle)
{
    std::vector<std::string> strs = { "abc" }; 
    auto r = fold_l(strs, [](auto a, auto b) { return a + ":"s + b; });
    EXPECT_EQ(r, "abc");
}

TEST(FpFold, LeftMultipleStrings)
{
    std::vector<std::string> strs = { "abc", "def", "ghi" };
    auto r = fold_l(strs, [](auto a, auto b) { return a + ":"s + b; });
    EXPECT_EQ(r, "abc:def:ghi");
}

TEST(FpFold, LeftMultipleInts)
{
    std::vector<int> values = { 1, 2, 3 };
    {
	auto r = fold_l(values, [](auto a, auto b) { return a + b; });
	EXPECT_EQ(r, 6);
    }
    {
	auto r = fold_l(values, 1, [](auto a, auto b) { return a + b; });
	EXPECT_EQ(r, 7);
    }
}

TEST(FpFold, Any)
{
    auto r0 = any(std::vector<bool>{}, [](const auto& elem) { return elem; });
    EXPECT_EQ(false, r0);
    
    auto r1 = any(std::vector<bool>{false}, [](const auto& elem) { return elem; });
    EXPECT_EQ(false, r1);
    
    auto r2 = any(std::vector<bool>{true}, [](const auto& elem) { return elem; });
    EXPECT_EQ(true, r2);
    
    auto r3 = any(std::vector<bool>{false, true}, [](const auto& elem) { return elem; });
    EXPECT_EQ(true, r3);
    
    auto r4 = any(std::vector<bool>{true, true}, [](const auto& elem) { return elem; });
    EXPECT_EQ(true, r4);
    
    auto r5 = any(std::vector<bool>{false, false}, [](const auto& elem) { return elem; });
    EXPECT_EQ(false, r5);
}

TEST(FpFold, All)
{
    auto r0 = all(std::vector<bool>{}, [](const auto& elem) { return elem; });
    EXPECT_EQ(true, r0);
    
    auto r1 = all(std::vector<bool>{false}, [](const auto& elem) { return elem; });
    EXPECT_EQ(false, r1);
    
    auto r2 = all(std::vector<bool>{true}, [](const auto& elem) { return elem; });
    EXPECT_EQ(true, r2);
    
    auto r3 = all(std::vector<bool>{false, true}, [](const auto& elem) { return elem; });
    EXPECT_EQ(false, r3);
    
    auto r4 = all(std::vector<bool>{true, true}, [](const auto& elem) { return elem; });
    EXPECT_EQ(true, r4);

    auto r5 = all(std::vector<bool>{false, false}, [](const auto& elem) { return elem; });
    EXPECT_EQ(false, r5);
}

int main(int argc, char *argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
