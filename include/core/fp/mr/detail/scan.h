// Copyright (C) 2021, 2022 by Mark Melton
//

#pragma once
#include <concepts>
#include "core/fp/mr/detail/interface.h"

namespace core::mr {

namespace detail {

template<class O, class A, class R>
struct ScanOutput : OutputBase<O> {
    ScanOutput(O&& output, A& acc, R& reducer)
	: OutputBase<O>(std::forward<O>(output))
	, acc_(acc)
	, reducer_(reducer) {
    }

    void operator()(const auto& elem) {
	acc_ = reducer_(acc_, elem);
	this->output_(acc_);
    }

private:
    A acc_;
    R& reducer_;
};

template<class O, class A, class R> ScanOutput(O&&, A&, R&) -> ScanOutput<O,A&,R&>;

template<Expression E, class A, class R>
struct Scan : Interface<Scan<E, A, R>> {
    using value_type = std::invoke_result_t<R, expr_value_t<E>, expr_value_t<E>>;
    
    Scan(E&& source, A&& acc, R&& reducer)
	: source_(std::forward<E>(source))
	, acc_(std::forward<A>(acc))
	, reducer_(std::forward<R>(reducer)) {
    }

    template<class O>
    auto compile(O&& output) {
	return source_.compile(ScanOutput{std::forward<O>(output), acc_, reducer_});
    }

    E source_;
    A acc_;
    R reducer_;
};

}; // detail

/// Returns a scan node
///
template<class A, class R>
auto scan(A& acc, R&& r) {
    return [acc = std::forward<A>(acc), r = std::forward<R>(r)]
	<detail::Expression E>(E&& expr) mutable {
	return detail::Scan{std::forward<decltype(expr)>(expr),
	    std::forward<A>(acc),
	    std::forward<R>(r)};
    };
};

}; // core::mr
