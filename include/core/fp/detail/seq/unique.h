// Copyright (C) 2022 by Mark Melton
//

#pragma once
#include <set>
#include "core/fp/detail/seq/interface.h"

namespace core::fp::detail {

template<Sequence S, class F>
struct Unique : Interface<Unique<S, F>> {
    using value_type = sequence_value_t<S>;
    using K = std::result_of_t<F(value_type)>;
    
    Unique(S&& source, F&& function)
	: seq_(std::forward<S>(source))
	, function_(std::forward<F>(function)) {
    }

    std::optional<value_type> next() {
	while (auto p = seq_.next()) {
	    auto key = function_(*p);
	    if (keys_.contains(key))
		continue;
	    keys_.insert(key);
	    return *p;
	}
	return std::nullopt;
    }
    
    S seq_;
    F function_;
    std::set<K> keys_;
};

}; // core::fp::detail
