// Copyright (C) 2021, 2022 by Mark Melton
//

#pragma once
#include "core/fp/mr/detail/interface.h"

namespace core::mr {

namespace detail {
		  
template<class F>
struct ApplyOutput {
    using Output = void;
    ApplyOutput(F function)
	: function_(function) {
    }

    void operator()(auto& elem) {
	function_(elem);
    }

private:
    F& function_;
};

template<class F> ApplyOutput(F&) -> ApplyOutput<F&>;

template<Expression E, class F>
struct Apply : Interface<Apply<E,F>> {
    using value_type = void;
    
    Apply(E&& source, F&& function)
	: source_(std::forward<E>(source))
	, function_(std::forward<F>(function)) {
    }

    auto compile() {
	return source_.compile(ApplyOutput{function_});
    }

    void compile(int);

    E source_;
    F function_;
};

template<class E, class F> Apply(E&&, F&) -> Apply<E,F&>;

}; // detail

/// Returns an apply node that applies the given ``function``.
///
template<class F>
auto apply(F&& function) {
    return [function = std::forward<F>(function)]<detail::Expression E>(E&& expr) mutable
	requires detail::Applicator<F,detail::expr_value_t<E>> {
	return detail::Apply{std::forward<E>(expr), std::forward<F>(function)};
    };
}

}; // core::mr
