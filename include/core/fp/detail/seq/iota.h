// Copyright (C) 2022 by Mark Melton
//

#pragma once
#include "core/fp/detail/seq/interface.h"

namespace core::fp::detail {

template<class T>
struct Iota : Interface<Iota<T>> {
    using value_type = T;

    Iota(size_t count, value_type start, value_type step)
	: count_(count)
	, value_(start)
	, step_(step) {
    }

    std::optional<value_type> next() {
	if (count_ > 0) {
	    auto r = value_;
	    value_ += step_;
	    --count_;
	    return r;
	}
	return std::nullopt;
    }

    size_t count_;
    value_type value_, step_;
};

}; // core::fp::detail

namespace core::fp {

template<class T = int>
auto iota(size_t count, T start = 0, T step = 1) {
    return detail::Iota{count, start, step};
}

}; // core::fp
