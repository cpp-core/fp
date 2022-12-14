// Copyright (C) 2022 by Mark Melton
//

#pragma once
#include "core/fp/detail/seq/interface.h"

namespace core::fp::detail {

template<Sequence S, class F> requires SequenceTransform<S, F>
struct Transform : Interface<Transform<S,F>> {
    using input_value_type = sequence_value_t<S>;
    using value_type = std::invoke_result_t<F, input_value_type>;

    Transform(S&& source, F&& function)
	: seq_(std::forward<S>(source))
	, function_(std::forward<F>(function)) {
    }

    Transform(const Transform&) = delete;
    Transform(Transform&&) = default;

    std::optional<value_type> next() {
	if (auto p = seq_.next())
	    return function_(*p);
	return std::nullopt;
    }

    S seq_;
    F function_;
};

}; // core::fp::detail

