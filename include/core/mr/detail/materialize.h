// Copyright (C) 2021 by Mark Melton
//

#pragma once
#include "core/mr/detail/source.h"
#include "core/mr/detail/sink.h"

namespace core::mr {

namespace detail {

template<Expression E>
struct Materialize {
    Materialize(E&& expr)
	: expr_(std::forward<E>(expr)) {
    }

    auto operator()() {
	Sink<expr_value_t<E>> result;
	auto cexpr = expr_.compile(result);
	cexpr.run();
	return result;
    }
    
    E expr_;
};

}; // detail

inline auto materialize() {
    return []<detail::Expression E>(E&& expr) {
	return detail::Materialize{std::forward<E>(expr)}();
    };
}

}; // core::mr
