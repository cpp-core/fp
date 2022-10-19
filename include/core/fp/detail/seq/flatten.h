// Copyright (C) 2022 by Mark Melton
//

#pragma once
#include "core/fp/detail/seq/sequences.h"
#include "core/tuple/map.h"

namespace core::fp::detail {

template<Sequence S>
struct Flatten;

template<Sequence S>
struct Flatten : Interface<Flatten<S>> {
    using tuple_type = sequence_value_t<S>;
    static constexpr auto tuple_size = std::tuple_size_v<tuple_type>;
    
    using value_type = std::tuple_element_t<0, tuple_type>;
    using iter_type = seq_iterator<S>;

    Flatten(S&& source)
	: source_(std::forward<S>(source))
	, index_(0)
	, tuple_(source_.next()) {
    }

    Flatten(const Flatten&) = delete;
    Flatten(Flatten&&) = default;

    std::optional<value_type> next() {
	if (tuple_) {
	    auto value = core::tp::map_nth([](auto& s) { return s; }, index_, *tuple_);
	    ++index_;
	    if (index_ >= tuple_size) {
		tuple_ = source_.next();
		index_ = 0;
	    }
	    return value;
	}

	return std::nullopt;
    }

    S source_;
    size_t index_;
    std::optional<tuple_type> tuple_;
};

}; // core::fp::detail
