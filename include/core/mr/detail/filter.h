// Copyright (C) 2021 by Mark Melton
//

#pragma once
#include "core/mr/detail/interface.h"

namespace core::mr {

namespace detail {

template<class P, class O>
struct FilterOutput : OutputBase<O> {
    FilterOutput(P& predicate, O&& output)
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

template<class P, class O> FilterOutput(P&, O&&) -> FilterOutput<P,O>;

template<Expression E, class P>
struct Filter : Interface<Filter<E,P>> {
    using value_type = std::result_of_t<P&(expr_value_t<E>)>;
    
    Filter(E&& source, P&& predicate)
	: source_(std::forward<E>(source))
	, predicate_(std::forward<P>(predicate)) {
    }

    template<class O>
    auto compile(O&& output) {
	return source_.compile(FilterOutput{predicate_, std::forward<O>(output)});
    }

    E source_;
    P predicate_;
};

}; // detail

template<class P>
auto filter(P predicate) {
    return [=]<detail::Expression E>(E&& expr) {
	return detail::Filter{std::forward<E>(expr), std::move(predicate)};
    };
};

}; // core::mr
