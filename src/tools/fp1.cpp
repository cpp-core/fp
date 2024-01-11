// Copyright (C) 2022, 2024 by Mark Melton
//

#include "core/fp/seq.h"
#include <iostream>

using std::cout;
using std::endl;
namespace fp = core::fp;

int main(int argc, const char *argv[]) {
    std::vector<int> numbers = {1, 2, 3};
    auto sum = fp::source(numbers)
                   .side_effect([](const auto &value) { cout << value << endl; })
                   .sum(0);
    cout << sum << endl;
    return 0;
}
