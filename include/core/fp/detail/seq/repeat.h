// Copyright (C) 2022 by Mark Melton
//

#pragma once
#include "core/fp/detail/seq/interface.h"

namespace core::fp::detail {

template<class T>
struct Repeat : Interface<Repeat<T>> {
    using value_type = std::decay_t<T>;

    Repeat(const T& value, size_t count)
	: value_(value)
	, count_(count) {
    }

    Repeat(const Repeat&) = delete;
    Repeat(Repeat&&) = default;
    
    std::optional<value_type> next() {
	if (count_ == 0)
	    return std::nullopt;
	--count_;
	return value_;
    }

    value_type value_;
    size_t count_;
};

}; // core::fp::detail

namespace core::fp {

template<class T>
auto repeat(const T& value, size_t count = std::numeric_limits<size_t>::max()) {
    return detail::Repeat{value, count};
}

}; // core::fp
