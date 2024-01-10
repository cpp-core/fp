// Copyright (C) 2022, 2024 by Mark Melton
//

#pragma once
#include "core/fp/detail/seq/interface.h"

namespace core::fp::detail {

template<Sequence S, class A, class R>
struct Visit : Interface<Visit<S, A, R>> {
    using value_type = sequence_value_t<S>;

    Visit(S&& seq, A&& acc, R&& reducer)
	: seq_(std::forward<S>(seq))
	, acc_(std::forward<A>(acc))
	, reducer_(std::forward<R>(reducer)) {
    }

    Visit(const Visit&) = delete;
    Visit(Visit&&) = default;

    const auto& run() {
	for (const auto& value : seq_)
	    reducer_(acc_, value);
	return acc_;
    }

    S seq_;
    A acc_;
    R reducer_;
};

}; // core::fp::detail
