// Copyright (C) 2021, 2022 by Mark Melton
//

#pragma once
#include "core/mr/detail/interface.h"

namespace core::mr {

namespace detail {
		  
template<class O, class A, class R>
struct ReduceOutput : OutputBase<O> {
    ReduceOutput(O&& output, A accumulator, R& reducer)
	: OutputBase<O>(std::forward<O>(output))
	, accumulator_(accumulator)
	, reducer_(reducer) {
    }

    void operator()(const auto& elem) {
	reducer_(accumulator_, elem);
    }

    auto& result() {
	return accumulator_;
    }

private:
    A accumulator_;
    R& reducer_;
};

template<class O, class A, class R> ReduceOutput(O&&, A, R&) -> ReduceOutput<O,A,R&>;

template<Expression E, class A, class R, class C = R>
struct Reduce : Interface<Reduce<E,A,R,C>> {
    using value_type = std::decay_t<A>;
    
    Reduce(E&& source, A&& accumulator, R&& reducer, C&& combiner)
	: source_(std::forward<E>(source))
	, accumulator_(std::forward<A>(accumulator))
	, reducer_(std::forward<R>(reducer))
	, combiner_(std::forward<C>(combiner)) {
    }

    Reduce(E&& source, A&& accumulator, R&& reducer)
	: source_(std::forward<E>(source))
	, accumulator_(std::forward<A>(accumulator))
	, reducer_(std::forward<R>(reducer)) {
    }

    template<class O>
    auto compile(O&& output) {
	return source_.compile(ReduceOutput{std::forward<O>(output),
					    accumulator_,
					    reducer_});
    }

    auto& source() {
	return source_;
    }

    const auto& initial_value() const {
	return accumulator_;
    }

    auto& reducer() {
	return reducer_;
    }

    auto& combiner() {
	return combiner_;
    }
    
    E source_;
    A accumulator_;
    R reducer_;
    C combiner_;
};

template<class E, class A, class R> Reduce(E&&, A&, R&&) -> Reduce<E,A&,R>;
template<class E, class A, class R, class C> Reduce(E&&, A&, R&&, C&&) -> Reduce<E,A&,R,C>;

}; // detail

template<class A, class R, class C>
auto reduce(A&& acc, R&& r, C&& c) {
    return [acc = std::forward<A>(acc), r = std::forward<R>(r), c = std::forward<C>(c)]
	<detail::Expression E>(E&& expr) mutable
	requires (detail::Reducer<E,A,R> and detail::Combiner<E,A,C>) {
	return detail::Reduce{std::forward<E>(expr),
	    std::forward<A>(acc),
	    std::forward<R>(r),
	    std::forward<C>(c)};
    };
}

/// Returns a *Reducer* node that applies the reducer ``r`` to update
/// the accumulator ``acc``.
///
template<class A, class R>
auto reduce(A&& acc, R&& r) {
    return [acc = std::forward<A>(acc), r = std::forward<R>(r)]
	<detail::Expression E>(E&& expr) mutable
	requires detail::Reducer<E, A, R> {
	return detail::Reduce{std::forward<E>(expr), std::forward<A>(acc), std::forward<R>(r)};
    };
}

}; // core::mr
