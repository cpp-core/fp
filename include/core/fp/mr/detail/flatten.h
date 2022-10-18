// Copyright (C) 2021, 2022 by Mark Melton
//

#pragma once
#include <concepts>
#include "core/fp/mr/detail/interface.h"
#include "core/fp/option.h"

namespace core::mr {

namespace detail {

template<class O>
struct FlattenOutput : OutputBase<O> {
    FlattenOutput(O&& output)
	: OutputBase<O>(std::forward<O>(output)) {
    }

    template<Container C>
    void operator()(const C& elems) {
	for (const auto& elem : elems)
	    this->output_(elem);
    }
    
    template<class T>
    void operator()(const Option<T>& elem) {
	if (elem)
	    this->output_(elem.get());
    }

private:
};

template<class O> FlattenOutput(O&&) -> FlattenOutput<O>;

template<Expression E>
struct Flatten : Interface<Flatten<E>> {
    using value_type = typename expr_value_t<E>::value_type;
    
    Flatten(E&& source)
	: source_(std::forward<E>(source)) {
    }

    template<class O>
    auto compile(O&& output) {
	return source_.compile(FlattenOutput{std::forward<O>(output)});
    }

    E source_;
};

}; // detail

/// Returns a flatten node
///
template<class P>
auto flatten() {
    return []<detail::Expression E>(E&& expr) {
	return detail::Flatten{std::forward<decltype(expr)>(expr)};
    };
};

}; // core::mr
