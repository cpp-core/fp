// Copyright (C) 2021 by Mark Melton
//

#pragma once
#include "core/mr/detail/util.h"

namespace core::mr {
struct Executor;
}; // core::mr

namespace core::mr::detail {

template<Expression, class>
struct Filter;

template<Expression>
struct Materialize;

template<detail::Expression>
struct Parallelize;

template<class>
struct Source;

template<Expression, class>
struct Transform;

template<class T>
struct Interface {
    
    template<class P>
    auto filter(P&& predicate) {
	return Filter{std::move(ref()), std::forward<P>(predicate)};
    }

    auto materialize() {
	return Materialize{std::move(ref())}();
    }

    auto parallelize(Executor& ex) {
	return Parallelize{std::move(ref()), ex}();
    }
    
    template<class F>
    auto transform(F&& function) {
	return Transform{std::move(ref()), std::forward<F>(function)};
    }

private:
    auto& ref() { return *static_cast<T*>(this); }
};

}; // core::mr::detail
