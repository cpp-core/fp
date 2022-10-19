// Copyright (C) 2022 by Mark Melton
//

#pragma once
#include "core/fp/detail/seq/interface.h"

namespace core::fp::detail {

template<Sequence S, class A, class R>
struct Fold : Interface<Fold<S, A, R>> {
    using value_type = sequence_value_t<S>;

    Fold(S&& seq, A&& acc, R&& reducer)
	: seq_(std::forward<S>(seq))
	, acc_(std::forward<A>(acc))
	, reducer_(std::forward<R>(reducer)) {
    }

    const auto& run() {
	for (const auto& value : seq_)
	    acc_ = reducer_(acc_, value);
	return acc_;
    }

    S seq_;
    A acc_;
    R reducer_;
};

}; // core::fp::detail
