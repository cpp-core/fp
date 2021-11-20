// Copyright (C) 2021 by Mark Melton
//

#pragma once
#include "core/mr/detail/sink.h"
#include "core/mp/same.h"

namespace core::mr {

namespace detail {

template<Expression E>
struct Materialize {
    Materialize(E&& expr)
	: expr_(std::forward<E>(expr)) {
    }

    auto operator()() {
	if constexpr (not core::mp::is_same_template_v<E, Apply>) {
	    Sink<expr_value_t<E>> result;
	    auto cexpr = expr_.compile(result);
	    return cexpr.run();
	}
	else {
	    auto cexpr = expr_.compile();
	    cexpr.apply();
	}
    }
    
    E expr_;
};

}; // detail

inline auto eval() {
    return []<detail::Expression E>(E&& expr) {
	return detail::Materialize{std::forward<E>(expr)}();
    };
}

}; // core::mr
