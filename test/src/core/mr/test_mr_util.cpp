// Copyright 2021, 2022 by Mark Melton
//

#include <gtest/gtest.h>
#include "core/mr/mr.h"
#include "coro/stream/stream.h"
#include "core/types/type_name.h"

using namespace core::mr::detail;

TEST(MapReduce, ContiguousIterator)
{
    EXPECT_FALSE(ContiguousIterator<int>);
    EXPECT_TRUE(ContiguousIterator<int*>);

    using ListIter = typename std::list<int>::iterator;
    using VectorIter = typename std::vector<int>::iterator;

    EXPECT_FALSE(ContiguousIterator<ListIter>);
    EXPECT_TRUE(ContiguousIterator<VectorIter>);
}

TEST(MapReduce, ContiguousContainer)
{
    std::vector<int> data{};
    auto s = Source{data};
    
    EXPECT_FALSE(ContiguousContainer<int>);
    EXPECT_FALSE(ContiguousContainer<coro::Generator<int>>);

    EXPECT_TRUE(ContiguousContainer<std::vector<int>>);
    EXPECT_TRUE(ContiguousContainer<std::vector<int>&>);
}

int main(int argc, char *argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
