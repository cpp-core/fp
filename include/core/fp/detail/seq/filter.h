// Copyright (C) 2022 by Mark Melton
//

#pragma once
#include "core/fp/detail/seq/interface.h"

namespace core::fp::detail {

template<Sequence S, SequencePredicate P>
struct Filter : Interface<Filter<S,P>> {
    using value_type = sequence_value_t<S>;

    Filter(S&& source, P&& predicate)
	: seq_(std::forward<S>(source))
	, predicate_(std::forward<P>(predicate)) {
    }

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
