// Copyright (C) 2022 by Mark Melton
//

#pragma once
#include "core/fp/detail/seq/interface.h"

namespace core::fp::detail {

template<Sequence S, class A, class R>
struct Scan : Interface<Scan<S, A, R>> {
    using value_type = A;
    
    Scan(S&& seq, A&& acc, R&& reducer)
	: seq_(std::forward<S>(seq))
	, acc_(std::forward<A>(acc))
	, reducer_(std::forward<R>(reducer)) {
    }

    std::optional<value_type> next() {
	if (auto p = seq_.next()) {
	    acc_ = reducer_(acc_, *p);
	    return acc_;
	}
	return std::nullopt;
    }

    S seq_;
    A acc_;    
    R reducer_;
};

}; // core::fp::detail
