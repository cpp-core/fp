// Copyright (C) 2022 by Mark Melton
//

#pragma once
#include "core/fp/detail/seq/interface.h"

namespace core::fp::detail {

template<Sequence S, class P> requires SequencePredicate<S, P>
struct Filter : Interface<Filter<S,P>> {
    using value_type = sequence_value_t<S>;

    Filter(S&& source, P&& predicate)
	: seq_(std::forward<S>(source))
	, predicate_(std::forward<P>(predicate)) {
    }

    Filter(const Filter&) = delete;
    Filter(Filter&&) = default;
    
    std::optional<value_type> next() {
	while (auto p = seq_.next())
	    if (predicate_(*p))
		return *p;
	return std::nullopt;
    }
    
    S seq_;
    P predicate_;
};

}; // core::fp::detail
