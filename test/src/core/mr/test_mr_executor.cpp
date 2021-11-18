// Copyright 2021 by Mark Melton
//

#include <gtest/gtest.h>
#include "core/mr/mr.h"
#include "core/mr/executor.h"
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

int main(int argc, char *argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    env = new Environment;
    AddGlobalTestEnvironment(env);
    return RUN_ALL_TESTS();
}
