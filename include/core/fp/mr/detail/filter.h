// Copyright (C) 2021, 2022 by Mark Melton
//

#pragma once
#include <concepts>
#include "core/fp/mr/detail/interface.h"

namespace core::mr {

namespace detail {

template<class O, class P>
struct FilterOutput : OutputBase<O> {
    FilterOutput(O&& output, P& predicate)
	: OutputBase<O>(std::forward<O>(output))
	, predicate_(predicate) {
    }

    void operator()(const auto& elem) {
	if (predicate_(elem))
	    this->output_(elem);
    }

private:
    P& predicate_;
};

template<class O, class P> FilterOutput(O&&, P&) -> FilterOutput<O,P&>;

template<Expression E, class P>
struct Filter : Interface<Filter<E,P>> {
    using value_type = expr_value_t<E>;
    
    Filter(E&& source, P&& predicate)
	: source_(std::forward<E>(source))
	, predicate_(std::forward<P>(predicate)) {
    }

    template<class O>
    auto compile(O&& output) {
	return source_.compile(FilterOutput{std::forward<O>(output), predicate_});
    }

    E source_;
    P predicate_;
};

}; // detail

/// Returns a filter node that applies the given ``predicate``.
///
template<class P>
auto filter(P&& predicate) {
    return [predicate = std::forward<P>(predicate)]
	<detail::Expression E>(E&& expr) mutable
	requires detail::Predicate<P, detail::expr_value_t<E>> {
	return detail::Filter{std::forward<decltype(expr)>(expr), std::forward<P>(predicate)};
    };
};

}; // core::mr
