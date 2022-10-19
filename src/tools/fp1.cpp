// Copyright (C) 2022 by Mark Melton
//

#include <iostream>
#include <list>
#include <set>
#include <vector>
#include "core/fp/seq.h"

using core::fp::source;
using namespace core::fp::chaining;
using namespace std::string_literals;

int main(int argc, const char *argv[]) {
    std::vector<std::string> strs = { "abc", "def", "ghi" };
    
    auto r = source(strs).fold(""s, [](auto a, auto b) { return a + ":"s + b; });

    for (auto n : r)
	std::cout << n << " ";
    std::cout << std::endl;

    return 0;
}

