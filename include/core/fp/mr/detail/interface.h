// Copyright (C) 2021, 2022 by Mark Melton
//

#pragma once
#include <optional>
#include "core/fp/mr/detail/util.h"

namespace core::mr {
struct Executor;
}; // core::mr

namespace core::mr::detail {

template<Expression, class>
struct Apply;

template<Expression, class>
struct Filter;

template<Expression>
struct Flatten;

template<Expression>
struct Materialize;

template<Expression>
struct Parallelize;

template<Expression,class,class,class>
struct Reduce;

template<class>
struct Source;

template<Expression, class>
struct Transform;

template<class T>
struct Interface {

    template<class F>
    auto apply(F&& function) {
	return Apply{std::move(ref()), std::forward<F>(function)};
    }
    
    template<class P>
    auto filter(P&& predicate) {
	return Filter{std::move(ref()), std::forward<P>(predicate)};
    }

    auto flatten() {
	return Flatten{std::move(ref())};
    }

    auto eval() {
	return Materialize{std::move(ref())}();
    }

    auto eval(Executor& ex, std::optional<size_t> max_chunk = std::nullopt) {
	return Parallelize{std::move(ref()), ex, max_chunk}();
    }

    template<class A, class R, class C>
    auto reduce(A&& accumulator, R&& reducer, C&& combiner) {
	return Reduce{std::move(ref()),
	    std::forward<A>(accumulator),
	    std::forward<R>(reducer),
	    std::forward<C>(combiner)};
    }
	
    template<class A, class R>
    auto reduce(A&& accumulator, R&& reducer) {
	return Reduce{std::move(ref()), std::forward<A>(accumulator), std::forward<R>(reducer)};
    }
	
    template<class F>
    auto transform(F&& function) {
	return Transform{std::move(ref()), std::forward<F>(function)};
    }

private:
    auto& ref() { return *static_cast<T*>(this); }
};

}; // core::mr::detail
