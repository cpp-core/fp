// Copyright 2021 by Mark Melton
//

#include <gtest/gtest.h>
#include "core/mr/mr.h"
#include "coro/stream/stream.h"

using namespace core;
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
};

Environment *env{nullptr};

TEST(MapReduce, Dot)
{
    auto data = env->iota(10);
    auto r = source(data)
	.filter([](int n) { return n % 2 == 0; })
	.transform([](int n) { return n * n; })
	.eval();
    EXPECT_EQ(r, (std::vector<int>{0, 4, 16, 36, 64}));
}

TEST(MapReduce, Pipe)
{
    auto data = env->iota(10);
    auto r = data
	| filter([](int n) { return n % 2 == 0; })
	| transform([](int n) { return n * n; })
	| eval();
    EXPECT_EQ(r, (std::vector<int>{0, 4, 16, 36, 64}));
}

TEST(MapReduce, DotEval)
{
    auto data = env->iota(10);
    auto r = source(data).eval();
    EXPECT_EQ(r, data);

    auto r2 = source(env->iota(10)).eval();
    EXPECT_EQ(r2, data);
}

TEST(MapReduce, PipeEval)
{
    auto data = env->iota(10);
    auto r = data | eval();
    EXPECT_EQ(r, data);
    
    // auto r2 = env->iota(10) | eval();
    // EXPECT_EQ(r2, data);
}

int main(int argc, char *argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    env = new Environment;
    AddGlobalTestEnvironment(env);
    return RUN_ALL_TESTS();
}
