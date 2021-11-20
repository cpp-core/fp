// Copyright 2021 by Mark Melton
//

#include <gtest/gtest.h>
#include "core/mr/mr.h"
#include "core/mr/executor.h"
#include "core/utility/fixed.h"
#include "coro/stream/stream.h"

using namespace core::mr;

class Environment : public ::testing::Environment {
public:
    Environment() : ex_(4) { }
    ~Environment() override { }
    void SetUp() override { }
    void TearDown() override { }
    auto iota(int size) { return coro::iota<int>(size) | coro::collect<std::vector>(); }
    Executor& executor() { return ex_; }
private:
    Executor ex_;
};

Environment *env{nullptr};

TEST(MapReduceExecutor, Dot)
{
    auto data = env->iota(10);
    auto r = source(data)
	.filter([](int n) { return n % 2 == 0; })
	.transform([](int n) { return n * n; })
	.eval(env->executor());
    EXPECT_EQ(r, (std::vector<int>{0, 4, 16, 36, 64}));
}

TEST(MapReduceExecutor, Pipe)
{
    auto data = env->iota(10);
    auto r = data
	| filter([](int n) { return n % 2 == 0; })
	| transform([](int n) { return n * n; })
	| eval(env->executor());
    EXPECT_EQ(r, (std::vector<int>{0, 4, 16, 36, 64}));
}

TEST(MapReduceExecutor, DotEval)
{
    auto data = env->iota(10);
    auto r = source(data).eval(env->executor());
    EXPECT_EQ(r, data);

    auto r2 = source(env->iota(10)).eval(env->executor());
    EXPECT_EQ(r2, data);
}

TEST(MapReduceExecutor, PipeEval)
{
    auto data = env->iota(10);
    auto r = data | eval(env->executor());
    EXPECT_EQ(r, data);
    
    auto r2 = env->iota(10) | eval(env->executor());
    EXPECT_EQ(r2, data);
}

TEST(MapReduceExecutor, DotFilter)
{
    auto data = env->iota(10);
    auto r = source(data).filter([](int n) { return true; }).eval(env->executor());
    EXPECT_EQ(r, data);

    auto r2 = source(env->iota(10)).filter([](int n) { return true; }).eval(env->executor());
    EXPECT_EQ(r2, data);
}

TEST(MapReduceExecutor, PipeFilter)
{
    auto data = env->iota(10);
    auto r = data | filter([](int n) { return true; }) | eval(env->executor());
    EXPECT_EQ(r, data);
    
    auto r2 = env->iota(10) | filter([](int n) { return true; }) | eval(env->executor());
    EXPECT_EQ(r2, data);
}

TEST(MapReduceExecutor, DotTransform)
{
    auto data = env->iota(10);
    auto r = source(data).transform([](int n) { return n; }).eval(env->executor());
    EXPECT_EQ(r, data);

    auto r2 = source(env->iota(10)).transform([](int n) { return n; }).eval(env->executor());
    EXPECT_EQ(r2, data);
}

TEST(MapReduceExecutor, PipeTransform)
{
    auto data = env->iota(10);
    auto r = data | transform([](int n) { return n; }) | eval(env->executor());
    EXPECT_EQ(r, data);
    
    auto r2 = env->iota(10) | transform([](int n) { return n; }) | eval(env->executor());
    EXPECT_EQ(r2, data);
}

TEST(MapReduceExecutor, DotReduce)
{
    auto data = env->iota(10);
    auto r = source(data)
	.reduce(0, [](int& acc, int n) { return acc += n; })
	.eval(env->executor())
	;
    EXPECT_EQ(r, 45);

    auto r2 = source(env->iota(10))
	.reduce(0, [](int& acc, int n) { return acc += n; })
	.eval(env->executor());
    EXPECT_EQ(r2, 45);
}

TEST(MapReduceExecutor, PipeReduce)
{
    auto data = env->iota(10);
    auto r = data
	| reduce(0, [](int& acc, int n) { return acc += n; })
	| eval(env->executor());
    EXPECT_EQ(r, 45);
    
    auto r2 = env->iota(10)
	| reduce(0, [](int& acc, int n) { return acc += n; })
	| eval(env->executor());
    EXPECT_EQ(r2, 45);
}

TEST(MapReduceExecutor, DotReduceMixed)
{
    std::pair<int,int> extrema{std::numeric_limits<int>::max(), std::numeric_limits<int>::min()};
    auto data = env->iota(10);
    auto r = source(data)
	.reduce(extrema, [](auto& acc, int n) {
	    acc.first = std::min(acc.first, n);
	    acc.second = std::max(acc.second, n);
	}, [](auto& acc, auto& other) {
	    acc.first = std::min(acc.first, other.first);
	    acc.second = std::max(acc.second, other.second);
	})
	.eval(env->executor());
    EXPECT_EQ(r.first, 0);
    EXPECT_EQ(r.second, 9);

    auto r2 = source(env->iota(10))
	.reduce(extrema, [](auto& acc, int n) {
	    acc.first = std::min(acc.first, n);
	    acc.second = std::max(acc.second, n);
	}, [](auto& acc, auto other) {
	    acc.first = std::min(acc.first, other.first);
	    acc.second = std::max(acc.second, other.second);
	})
	.eval(env->executor());
    EXPECT_EQ(r2.first, 0);
    EXPECT_EQ(r2.second, 9);
}

TEST(MapReduceExecutor, PipeReduceMixed)
{
    std::pair<int,int> extrema{std::numeric_limits<int>::max(), std::numeric_limits<int>::min()};
    auto data = env->iota(10);
    auto r = data
	| reduce(extrema, [](auto& acc, int n) {
	    acc.first = std::min(acc.first, n);
	    acc.second = std::max(acc.second, n);
	}, [](auto& acc, auto& other) {
	    acc.first = std::min(acc.first, other.first);
	    acc.second = std::max(acc.second, other.second);
	})
	| eval(env->executor());
    EXPECT_EQ(r.first, 0);
    EXPECT_EQ(r.second, 9);

    auto r2 = env->iota(10)
	| reduce(extrema, [](auto& acc, int n) {
	    acc.first = std::min(acc.first, n);
	    acc.second = std::max(acc.second, n);
	}, [](auto& acc, auto& other) {
	    acc.first = std::min(acc.first, other.first);
	    acc.second = std::max(acc.second, other.second);
	})
	| eval(env->executor());
    EXPECT_EQ(r2.first, 0);
    EXPECT_EQ(r2.second, 9);
}

TEST(MapReduceExecutor, DotApply)
{
    auto data = env->iota(10);
    source(data).apply([](int& n) { n *= 2; }).eval(env->executor());
    int count{0};
    for (auto& elem : data) {
	EXPECT_EQ(elem, 2 * count);
	++count;
    }

    int sum2{0};
    source(env->iota(10)).apply([&](int& n) { sum2 += n; }).eval(env->executor());
}

TEST(MapReduceExecutor, PipeApply)
{
    auto data = env->iota(10);

    data | apply([](int& n) { n *= 2; }) | eval(env->executor());
    int count{0};
    for (auto& elem : data) {
	EXPECT_EQ(elem, 2 * count);
	++count;
    }

    int sum2{0};
    env->iota(10) | apply([&](int& n) { sum2 += n; }) | eval(env->executor());
}

int main(int argc, char *argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    env = new Environment;
    AddGlobalTestEnvironment(env);
    return RUN_ALL_TESTS();
}
