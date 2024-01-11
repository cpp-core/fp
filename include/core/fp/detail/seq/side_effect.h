// Copyright (C) 2022, 2024 by Mark Melton
//

#pragma once
#include "core/fp/detail/seq/interface.h"

namespace core::fp::detail {

template<Sequence S, class F>
struct SideEffect : Interface<SideEffect<S, F>> {
    using value_type = sequence_value_t<S>;

    SideEffect(S&& seq, F&& function)
	: seq_(std::forward<S>(seq))
	, function_(std::forward<F>(function)) {
    }

    SideEffect(const SideEffect&) = delete;
    SideEffect(SideEffect&&) = default;

    std::optional<value_type> next() {
	while (auto p = seq_.next()) {
	    function_(*p);
	    return *p;
	}
	return std::nullopt;
    }

    S seq_;
    F function_;
};

template<class S, class F> SideEffect(S&&, F&&) -> SideEffect<S, F>;

}; // core::fp::detail
