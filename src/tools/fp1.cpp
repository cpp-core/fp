// Copyright (C) 2022, 2024 by Mark Melton
//

#include <iostream>
#include "core/fp/seq.h"

using std::cout, std::endl;
namespace fp = core::fp;

int main(int argc, const char *argv[]) {
    std::vector<int> numbers = {1, 2, 3};
    auto ll = [](auto& state, auto item) { state += item; };
    
    cout << core::fp::detail::Visit{fp::source(numbers), int{0}, std::move(ll)}.run() << endl;
    
    int m{2};
    cout << core::fp::detail::Visit{fp::source(numbers), m, std::move(ll)}.run() << endl;
    std::cout << m << std::endl;
    return 0;
}

