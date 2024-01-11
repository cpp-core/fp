// Copyright (C) 2022, 2024 by Mark Melton
//

#pragma once
#include "core/fp/detail/seq/interface.h"

namespace core::fp::detail {

template<Sequence S, class F>
struct Apply : Interface<Apply<S, F>> {
    using value_type = sequence_value_t<S>;

    Apply(S&& seq, F&& function)
	: seq_(std::forward<S>(seq))
	, function_(std::forward<F>(function)) {
    }

    Apply(const Apply&) = delete;
    Apply(Apply&&) = default;

    void run() {
	for (const auto& value : seq_)
	    function_(value);
    }

    S seq_;
    F function_;
};

template<class S, class F> Apply(S&&, F&&) -> Apply<S, F>;

}; // core::fp::detail
