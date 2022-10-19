// Copyright (C) 2022 by Mark Melton
//

#pragma once
#include "core/fp/detail/seq/interface.h"

namespace core::fp::detail {

template<Sequence S>
struct Take : Interface<Take<S>> {
    using value_type = sequence_value_t<S>;

    Take(S&& seq, size_t count)
	: seq_(std::forward<S>(seq))
	, count_(count) {
    }

    Take(const Take&) = delete;
    Take(Take&&) = default;
    
    std::optional<value_type> next() {
	if (count_ > 0) {
	    --count_;
	    if (auto p = seq_.next())
		return *p;
	}
	return std::nullopt;
    }

    S seq_;
    size_t count_;
};

}; // core::fp::detail
