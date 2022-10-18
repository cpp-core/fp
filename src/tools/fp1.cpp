// Copyright (C) 2022 by Mark Melton
//

#include <iostream>
#include <set>
#include <vector>
#include "core/fp/mr/mr.h"
#include "core/fp/option.h"

using namespace core::fp;
using namespace core::mr;

int main(int argc, const char *argv[]) {
    std::vector<int> values{ 1, 2, 3, 4, 5 };
    
    auto flat_values = source(values)
	.scan(1, [](int a, int b) { return a * b; })
	.eval()
	;
    
    std::cout << std::endl;
    for (auto n : flat_values)
	std::cout << n << std::endl;
    return 0;
}
