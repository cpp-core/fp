// Copyright 2021, 2022 by Mark Melton
//

#include <gtest/gtest.h>
#include "core/fp/seq.h"
#include "coro/stream/stream.h"

using namespace std::string_literals;

template<class T>
class Fixed : public T {
    using T::T;
    Fixed(const Fixed&) = delete;
    Fixed(Fixed&&) = delete;

    Fixed& operator=(const Fixed&) = delete;
    Fixed& operator=(Fixed&&) = delete;
};

using namespace core::fp;
using namespace core::fp::chaining;

class Environment : public ::testing::Environment {
public:
    Environment() { }
    ~Environment() override { }
    void SetUp() override { }
    void TearDown() override { }
    auto iota(int size) {
	return coro::iota<int>(size) | coro::collect<std::vector>();
    }

    void iota(std::vector<int>& data, int size) {
	for (auto elem : this->iota(size))
	    data.push_back(elem);
    }
};

Environment *env{nullptr};

TEST(FpSeq, DotAlternate)
{
    std::vector<int> expected = { 0, 0, 2, 1, 4, 2, 6, 8 };
    auto s0 = source(env->iota(10)).filter([](int n) { return n % 2 == 0; });
    auto s1 = source(env->iota(3));
    auto r = (std::move(s0) * std::move(s1)).alternate().collect();
    EXPECT_EQ(r, expected);
}

TEST(FpSeq, DotChoose)
{
    std::vector<int> expected = { 0, 0, 1, 2, 2, 4, 6, 8 };
    std::vector<int> not_expected = { 0, 0, 2, 1, 4, 2, 6, 8 };
    auto s0 = source(env->iota(10)).filter([](int n) { return n % 2 == 0; });
    auto s1 = source(env->iota(3));
    auto r = (std::move(s0) * std::move(s1)).choose().collect();
    EXPECT_NE(r, not_expected);
    
    std::sort(r.begin(), r.end());
    EXPECT_EQ(r, expected);
}

TEST(FpSeq, DotCollect)
{
    Fixed<std::vector<int>> data;
    env->iota(data, 10);
    std::list<int> lst(data.begin(), data.end());
    
    auto r = source(data).collect<std::list>();
    EXPECT_EQ(r, lst);

    auto r2 = source(env->iota(10)).collect<std::vector>();
    EXPECT_EQ(r2, data);
}

TEST(FpSeq, DotConcat)
{
    std::vector<int> expected = { 0, 2, 4, 6, 8, 0, 1, 2 };
    auto s0 = source(env->iota(10)).filter([](int n) { return n % 2 == 0; });
    auto s1 = source(env->iota(3));
    auto r = (std::move(s0) * std::move(s1)).concat().collect();
    EXPECT_EQ(r, expected);
}

TEST(FpSeq, DotFilter)
{
    Fixed<std::vector<int>> data;
    env->iota(data, 10);

    auto r = source(data).filter([](int n) { return true; }).collect();
    EXPECT_EQ(r, data);

    auto r2 = source(env->iota(10)).filter([](int n) { return true; }).collect();
    EXPECT_EQ(r2, data);
}

TEST(FpSeq, DotFlatten)
{
    std::vector<int> expected = {0, 1, 2, 3, 4, 5};
    std::vector<std::tuple<int,int>> data = {{ 0, 1}, {2, 3}, {4, 5}};
    auto r = source(data).flatten().collect();
    EXPECT_EQ(r, expected);
}

TEST(FpSeq, DotFold)
{
    Fixed<std::vector<int>> data;
    env->iota(data, 10);

    auto r = source(data).fold(0, [](int s, int n) { return s + n; });
    EXPECT_EQ(r, 45);

    auto r2 = source(data).fold(0, [](int s, int n) { return s + n; });
    EXPECT_EQ(r2, 45);
}

TEST(FpSeq, DotIota)
{
    std::vector<int> expected = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
    auto r = iota(10).collect();
    EXPECT_EQ(r, expected);
    
    {
	std::vector<int> expected = { 11, 12, 13, 14 };
	auto r = iota(4, 11).collect();
	EXPECT_EQ(r, expected);
    }
    
    {
	std::vector<int> expected = { 1, 3, 5, 7, 9, 11, 13 };
	auto r = iota(7, 1, 2).collect();
	EXPECT_EQ(r, expected);
    }
}

TEST(FpSeq, DotJoin)
{
    std::vector<std::string> strs = { "abc", "def", "ghi" };
    auto r = source(strs).join(",");
    EXPECT_EQ(r, "abc,def,ghi"s);
}

TEST(FpSeq, DotOnce)
{
    auto r = once(42).collect();
    EXPECT_EQ(r, std::vector<int>{ 42 });
}

TEST(FpSeq, DotRepeat)
{
    std::vector<int> expected = { 42, 42, 42 };
    auto r = repeat(42, 3).collect();
    EXPECT_EQ(r, expected);
}

TEST(FpSeq, DotScan)
{
    Fixed<std::vector<int>> data;
    env->iota(data, 10);

    int s = 0;
    std::vector<int> expected;
    for (auto n : data) {
	s += n;
	expected.push_back(s);
    }

    auto r = source(data)
	.scan(0, [](int a, int n) { return a + n; })
	.collect();
    EXPECT_EQ(r, expected);

    auto r2 = source(env->iota(10))
	.scan(0, [](int a, int n) { return a + n; })
	.collect();
    EXPECT_EQ(r2, expected);
}

TEST(FpSeq, DotSource)
{
    Fixed<std::vector<int>> data;
    env->iota(data, 10);
    
    auto r = source(data)
	.filter([](int n) { return n % 2 == 0; })
	.transform([](int n) { return n * n; })
	.collect();
    EXPECT_EQ(r, (std::vector<int>{0, 4, 16, 36, 64}));
}

TEST(FpSeq, DotTake)
{
    Fixed<std::vector<int>> data;
    env->iota(data, 10);
    auto expected = env->iota(6);

    auto r = source(data).take(6).collect();
    EXPECT_EQ(r, expected);

    auto r2 = source(env->iota(10)).take(6).collect();
    EXPECT_EQ(r2, expected);
}

TEST(FpSeq, DotTransform)
{
    Fixed<std::vector<int>> data;
    env->iota(data, 10);

    auto r = source(data).transform([](int n) { return n; }).collect();
    EXPECT_EQ(r, data);

    auto r2 = source(env->iota(10)).transform([](int n) { return n; }).collect();
    EXPECT_EQ(r2, data);
}

TEST(FpSeq, DotUnique)
{
    std::vector<int> expected = { 0, 2, 4, 6, 8, 3, 9 };
    auto r = (iota(5, 0, 2) * iota(4, 0, 3)).concat().unique().collect();
    EXPECT_EQ(r, expected);

    {
	std::vector<int> expected = { 4, 5, 6 };
	auto r = iota(10, 4).unique([](int n) { return n % 3; }).collect();
	EXPECT_EQ(r, expected);
    }
}

TEST(FpSeq, DotZip)
{
    Fixed<std::vector<int>> data;
    env->iota(data, 10);
    
    std::vector<std::tuple<int,int>> expected;
    for (auto n : data)
	expected.emplace_back(n, n);

    auto r = (source(data) * source(data)).zip().collect();
    EXPECT_EQ(r, expected);

    auto r2 = (source(env->iota(10)) * source(env->iota(10))).zip().collect();
    EXPECT_EQ(r2, expected);
}

int main(int argc, char *argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    env = new Environment;
    AddGlobalTestEnvironment(env);
    return RUN_ALL_TESTS();
}
