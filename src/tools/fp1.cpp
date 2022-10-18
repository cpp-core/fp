// Copyright (C) 2022 by Mark Melton
//

#include <iostream>
#include <list>
#include <set>
#include <vector>
#include "core/fp/seq.h"

using core::fp::source;
using namespace core::fp::chaining;

int main(int argc, const char *argv[]) {
    std::vector<int> values{ 1, 2, 3, 4, 5, 6, 7, 8, 9 };

    auto data = source(values)
	.filter([](int n) { return n % 2 < 2; })
	.transform([](int n) { return n; })
	.scan(0, [](int acc, int n) { return acc + n; })
	.collect<std::list>()
	;
    for (auto n : data)
	std::cout << n << " ";
    std::cout << std::endl;

    auto z = (source(values) * source(values) * source(values)).zip().take(5);
    for (auto [a, b, c] : z)
	std::cout << a << "," << b << "," << c << "  " ;
    std::cout << std::endl;
    
    return 0;
}

