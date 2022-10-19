// Copyright 2021, 2022 by Mark Melton
//

#include <gtest/gtest.h>
#include "core/fp/try.h"
#include "core/fp/either.h"
#include "core/fp/match.h"
#include "core/fp/option.h"

using namespace core::fp;
using namespace std::literals::string_literals;

TEST(FpTry, ConstructSuccess)
{
    Success s{42};
    ASSERT_TRUE(s);
    EXPECT_EQ(s.get(), 42);
}

TEST(FpTry, ConstructFailure)
{
    Failure f{};
    EXPECT_FALSE(f);

    Failure f0{std::runtime_error("bad")};
    ASSERT_FALSE(f0);
    EXPECT_THROW(f0.get_failure().rethrow(), std::exception);

    Failure f1{std::runtime_error("bad")};
    EXPECT_EQ(f1.get_failure().what(), "std::exception"s);
}

TEST(FpTry, Construct)
{
    Try<int> s = Success{42};
    ASSERT_TRUE(s);
    EXPECT_EQ(s.get(), 42);

    Try<int> f = Failure{};
    EXPECT_FALSE(f);
}

TEST(FpTry, IsSuccessFailure)
{
    Success s{42};
    ASSERT_TRUE(s);
    EXPECT_TRUE(s.is_success());
    EXPECT_FALSE(s.is_failure());

    Failure f;
    ASSERT_FALSE(f);
    EXPECT_TRUE(f.is_failure());
    EXPECT_FALSE(f.is_success());
}

TEST(FpTry, Filter)
{
    Success s{42};
    auto t = s.filter([](int elem) { return elem == 42; });
    ASSERT_TRUE(t);
    EXPECT_EQ(t.get(), 42);

    auto t0 = s.filter([](int elem) { return elem != 42; });
    EXPECT_FALSE(t0);

    Failure<int> f{};
    auto t1 = f.filter([](int elem) { return elem != 42; });
    EXPECT_FALSE(t1);
    
    auto t2 = f.filter([](int elem) { return elem == 42; });
    EXPECT_FALSE(t2);
}

TEST(FpTry, FlatMap)
{
    Success s{21};
    auto t = s.flat_map([](int elem) { return Success{2 * elem}; });
    ASSERT_TRUE(t);
    EXPECT_EQ(t.get(), 42);

    auto t0 = s.flat_map([](int elem) { return Failure<int>{}; });
    EXPECT_FALSE(t0);

    Failure<int> f{};
    auto t1 = f.flat_map([](int elem) { return Success{2 * elem}; });
    EXPECT_FALSE(t1);
    
    auto t2 = f.flat_map([](int elem) { return Failure<int>{}; });
    EXPECT_FALSE(t2);
}

TEST(FpTry, Fold)
{
    Success s{21};
    Failure<int> f{};
    auto r0 = s.fold([](const TryFailure&) { return 21; },
		     [](int elem) { return 2 * elem; });
    EXPECT_EQ(r0, 42);

    auto r1 = f.fold([](const TryFailure&) { return 42; },
		     [](int elem) { return elem; });
    EXPECT_EQ(r1, 42);
}

TEST(FpTry, ForEach)
{
    Success s{21};
    s.foreach([](int& elem) { elem *= 2; });
    ASSERT_TRUE(s);
    EXPECT_EQ(s.get(), 42);

    Failure<int> f{};
    f.foreach([](int& elem) { elem *= 2; });
    EXPECT_FALSE(f);
}

TEST(FpTry, GetOrElse)
{
    Success s{42};
    EXPECT_EQ(s.get_or_else(21), 42);

    Failure<int> f{};
    EXPECT_EQ(f.get_or_else(42), 42);
}

TEST(FpTry, Map)
{
    Success s{21};
    auto r0 = s.map([](int elem) { return 2 * elem; });
    ASSERT_TRUE(r0);
    EXPECT_EQ(r0.get(), 42);

    Failure<int> f{};
    auto r1 = f.map([](int elem) { return 2 * elem; });
    EXPECT_FALSE(r1);
}

TEST(FpTry, Match)
{
    Success s{42};
    match(s,
	  [](const TrySuccess<int>& value) {
	      EXPECT_EQ(value.get(), 42);
	  },
	  [](const TryFailure& fail) {
	      ASSERT_TRUE(false);
	  });
    
    Failure<int> f;
    match(f,
	  [](const TrySuccess<int>& value) {
	      ASSERT_TRUE(false);
	  },
	  [](const TryFailure& fail) {
	      ASSERT_TRUE(true);
	  });
}

TEST(FpTry, OrElse)
{
    Success s{42};
    auto r0 = s.or_else(Success{21});
    ASSERT_TRUE(r0);
    EXPECT_EQ(r0.get(), 42);
    
    Failure<int> f{};
    auto r1 = f.or_else(Success{42});
    ASSERT_TRUE(r1);
    EXPECT_EQ(r1.get(), 42);
}

TEST(FpTry, ToEither)
{
    Success s{42};
    auto r0 = s.to_either();
    ASSERT_TRUE(r0.is_right());
    EXPECT_EQ(r0.get_right(), 42);

    Failure f{};
    auto r1 = f.to_either();
    ASSERT_TRUE(r1.is_left());
}

TEST(FpTry, ToOption)
{
    Success s{42};
    auto r0 = s.to_option();
    ASSERT_TRUE(r0);
    EXPECT_EQ(r0.get(), 42);

    Failure f{};
    auto r1 = f.to_option();
    ASSERT_FALSE(r1);
}

TEST(FpTry, Transform)
{
    Success s{21};
    auto r0 = s.transform([](const auto& f) { return Success{0}; },
			  [](int elem) { return Success{2 * elem}; });
    ASSERT_TRUE(r0);
    EXPECT_EQ(r0.get(), 42);

    Failure<int> f{};
    auto r1 = f.transform([](const auto& f) { return Success{42}; },
			  [](int elem) { return Success{0}; });
    ASSERT_TRUE(r1);
    EXPECT_EQ(r1.get(), 42);
}

int main(int argc, char *argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
