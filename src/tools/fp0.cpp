// Copyright (C) 2022, 2024 by Mark Melton
//

#include <iostream>
#include <set>
#include <vector>
#include "core/fp/mr/mr.h"
#include "core/fp/option.h"

using namespace core::fp;
using namespace core::mr;

int main(int argc, const char *argv[]) {
    std::vector<Option<int>> data{ Some{0}, Some{1}, Some{2}, None{}, Some{4} };
    auto squares = source(data)
	.flatten()
	.eval();
    std::cout << std::endl;
    for (auto n : squares)
	std::cout << n << std::endl;

    std::vector<std::set<int>> values{ { 0, 1, 2 }, { 3, 4, 5 } };
    auto flat_values = source(values)
	.flatten()
	.eval()
	;
    std::cout << std::endl;
    for (auto n : flat_values)
	std::cout << n << std::endl;
    return 0;
}
