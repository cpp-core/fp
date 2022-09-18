// Copyright 2021, 2022 by Mark Melton
//

#include <gtest/gtest.h>
#include "core/fp/mr/mr.h"
#include "coro/stream/stream.h"
#include "core/util/fixed.h"

using namespace core::mr;

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

TEST(MapReduce, Dot)
{
    core::Fixed<std::vector<int>> data;
    env->iota(data, 10);
    
    auto r = source(data)
	.filter([](int n) { return n % 2 == 0; })
	.transform([](int n) { return n * n; })
	.eval();
    EXPECT_EQ(r, (std::vector<int>{0, 4, 16, 36, 64}));
}

TEST(MapReduce, Pipe)
{
    core::Fixed<std::vector<int>> data;
    env->iota(data, 10);

    auto r = data
	| filter([](int n) { return n % 2 == 0; })
	| transform([](int n) { return n * n; })
	| eval();
    EXPECT_EQ(r, (std::vector<int>{0, 4, 16, 36, 64}));
}

TEST(MapReduce, DotEval)
{
    core::Fixed<std::vector<int>> data;
    env->iota(data, 10);

    auto r = source(data).eval();
    EXPECT_EQ(r, data);

    auto r2 = source(env->iota(10)).eval();
    EXPECT_EQ(r2, data);
}

TEST(MapReduce, PipeEval)
{
    core::Fixed<std::vector<int>> data;
    env->iota(data, 10);

    auto r = data | eval();
    EXPECT_EQ(r, data);
    
    auto r2 = env->iota(10) | eval();
    EXPECT_EQ(r2, data);
}

TEST(MapReduce, DotFilter)
{
    core::Fixed<std::vector<int>> data;
    env->iota(data, 10);

    auto r = source(data).filter([](int n) { return true; }).eval();
    EXPECT_EQ(r, data);

    auto r2 = source(env->iota(10)).filter([](int n) { return true; }).eval();
    EXPECT_EQ(r2, data);
}

TEST(MapReduce, PipeFilter)
{
    core::Fixed<std::vector<int>> data;
    env->iota(data, 10);

    auto r = data | filter([](int n) { return true; }) | eval();
    EXPECT_EQ(r, data);
    
    auto r2 = env->iota(10) | filter([](int n) { return true; }) | eval();
    EXPECT_EQ(r2, data);
}

TEST(MapReduce, DotTransform)
{
    core::Fixed<std::vector<int>> data;
    env->iota(data, 10);

    auto r = source(data).transform([](int n) { return n; }).eval();
    EXPECT_EQ(r, data);

    auto r2 = source(env->iota(10)).transform([](int n) { return n; }).eval();
    EXPECT_EQ(r2, data);
}

TEST(MapReduce, PipeTransform)
{
    core::Fixed<std::vector<int>> data;
    env->iota(data, 10);

    auto r = data | transform([](int n) { return n; }) | eval();
    EXPECT_EQ(r, data);
    
    auto r2 = env->iota(10) | transform([](int n) { return n; }) | eval();
    EXPECT_EQ(r2, data);
}

TEST(MapReduce, DotReduce)
{
    core::Fixed<std::vector<int>> data;
    env->iota(data, 10);

    auto r = source(data).reduce(0, [](int& acc, int n) { acc += n; }).eval();
    EXPECT_EQ(r, 45);

    auto r2 = source(env->iota(10)).reduce(0, [](int& acc, int n) { acc += n; }).eval();
    EXPECT_EQ(r2, 45);
}

TEST(MapReduce, PipeReduce)
{
    core::Fixed<std::vector<int>> data;
    env->iota(data, 10);

    auto r = data | reduce(0, [](int& acc, int n) { acc += n; }) | eval();
    EXPECT_EQ(r, 45);
    
    auto r2 = env->iota(10) | reduce(0, [](int& acc, int n) { acc += n; }) | eval();
    EXPECT_EQ(r2, 45);
}

TEST(MapReduce, DotReduceMixed)
{
    std::pair<int,int> extrema{std::numeric_limits<int>::max(), std::numeric_limits<int>::min()};
    auto data = env->iota(10);
    auto r = source(data)
	.reduce(extrema, [](auto& acc, int n) {
	    acc.first = std::min(acc.first, n);
	    acc.second = std::max(acc.second, n);
	})
	.eval();
    EXPECT_EQ(r.first, 0);
    EXPECT_EQ(r.second, 9);

    auto r2 = source(env->iota(10))
	.reduce(extrema, [](auto& acc, int n) {
	    acc.first = std::min(acc.first, n);
	    acc.second = std::max(acc.second, n);
	})
	.eval();
    EXPECT_EQ(r2.first, 0);
    EXPECT_EQ(r2.second, 9);
}

TEST(MapReduce, PipeReduceMixed)
{
    core::Fixed<std::vector<int>> data;
    env->iota(data, 10);

    std::pair<int,int> extrema{std::numeric_limits<int>::max(), std::numeric_limits<int>::min()};
    auto r = data
	| reduce(extrema, [](auto& acc, int n) {
	    acc.first = std::min(acc.first, n);
	    acc.second = std::max(acc.second, n);
	})
	| eval();
    EXPECT_EQ(r.first, 0);
    EXPECT_EQ(r.second, 9);

    auto r2 = env->iota(10)
	| reduce(extrema, [](auto& acc, int n) {
	    acc.first = std::min(acc.first, n);
	    acc.second = std::max(acc.second, n);
	})
	| eval();
    EXPECT_EQ(r2.first, 0);
    EXPECT_EQ(r2.second, 9);
}

TEST(MapReduce, DotApply)
{
    core::Fixed<std::vector<int>> data;
    env->iota(data, 10);

    int sum{0};
    source(data).apply([&](int& n) { n *= 2; sum += n; }).eval();
    EXPECT_EQ(sum, 90);
    int count{0};
    for (auto& elem : data) {
	EXPECT_EQ(elem, 2 * count);
	++count;
    }

    int sum2{0};
    source(env->iota(10)).apply([&](int& n) { sum2 += n; }).eval();
    EXPECT_EQ(sum2, 45);
}

TEST(MapReduce, PipeApply)
{
    core::Fixed<std::vector<int>> data;
    env->iota(data, 10);

    int sum{0};
    data | apply([&](int& n) { n *= 2; sum += n; }) | eval();
    EXPECT_EQ(sum, 90);
    int count{0};
    for (auto& elem : data) {
	EXPECT_EQ(elem, 2 * count);
	++count;
    }

    int sum2{0};
    env->iota(10) | apply([&](int& n) { sum2 += n; }) | eval();
    EXPECT_EQ(sum2, 45);
}

int main(int argc, char *argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    env = new Environment;
    AddGlobalTestEnvironment(env);
    return RUN_ALL_TESTS();
}
