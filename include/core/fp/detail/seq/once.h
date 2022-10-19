// Copyright (C) 2022 by Mark Melton
//

#pragma once
#include "core/fp/detail/seq/interface.h"

namespace core::fp::detail {

template<class T>
struct Once : Interface<Once<T>> {
    using value_type = T;

    Once(const T& value)
	: value_(value) {
    }

    std::optional<value_type> next() {
	if (exhausted_)
	    return std::nullopt;
	exhausted_ = true;
	return value_;
    }

    value_type value_;
    bool exhausted_{false};
};

}; // core::fp::detail

namespace core::fp {

template<class T>
auto once(const T& value) {
    return detail::Once{value};
}

}; // core::fp
