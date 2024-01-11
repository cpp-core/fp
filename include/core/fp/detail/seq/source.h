// Copyright (C) 2022, 2024 by Mark Melton
//

#pragma once
#include "core/fp/detail/seq/interface.h"

namespace core::fp::detail {

template<class C>
struct Source : Interface<Source<C>> {
    using value_type = typename std::decay_t<C>::value_type;
    using iter_type = decltype(std::begin(std::declval<C>()));
    Source(std::add_rvalue_reference_t<C> data)
	: data_(std::forward<C>(data))
	, iter_(data_.begin()) {
    }

    Source(const Source& other) = delete;
    Source(Source&& other) = default;

    std::optional<value_type> next() {
	if (iter_ != data_.end())
	    return *iter_++;
	return std::nullopt;
    }

private:
    C data_;
    iter_type iter_;
};

template<class C> Source(C&&) -> Source<C>;

}; // core::fp::detail

namespace core::fp {

// Adapt container `C` to be used as the source for a chain of
// operations.
template<class C>
auto source(C&& data) {
    return detail::Source{std::forward<C>(data)};
}

}; // core::fp
