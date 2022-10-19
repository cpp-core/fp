// Copyright (C) 2022 by Mark Melton
//

#include <iostream>
#include <list>
#include <set>
#include <vector>
#include "core/fp/seq.h"

using core::fp::source;
using namespace core::fp::chaining;

auto getdata() {
    return std::vector<int>{1, 2, 3};
}

int main(int argc, const char *argv[]) {
    std::vector<int> values{ 1, 2, 3, 4, 5, 6, 7, 8, 9 };

    auto r = source(values).filter([](int n) { return true; }).collect();

    for (auto n : r)
	std::cout << n << " ";
    std::cout << std::endl;

    return 0;
}

