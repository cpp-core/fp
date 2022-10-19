// Copyright (C) 2022 by Mark Melton
//

#pragma once
#include <random>
#include <set>
#include "core/fp/detail/seq/sequences.h"
#include "core/tuple/map.h"

namespace core::fp::detail {

template<Sequence S, Sequence... Ss>
struct Choose : Interface<Choose<S, Ss...>> {
    using source_type = Sequences<S, Ss...>;
    using value_type = sequence_value_t<S>;
    static constexpr auto N = 1 + sizeof...(Ss);

    Choose(source_type&& tuple)
	: source_(std::forward<source_type>(tuple))
	, dist_(0, N - 1) {
    }

    std::optional<value_type> next() {
	using core::tp::map_nth;
	while (exhausted_.size() < N) {
	    auto idx = dist_(rng_);
	    if (exhausted_.contains(idx))
		continue;
	    if (auto v = map_nth([](auto& s) { return s.next(); }, idx, source_.ref()))
		return *v;
	    exhausted_.insert(idx);
	}
	return std::nullopt;
    }

    source_type source_;
    std::uniform_int_distribution<size_t> dist_;
    std::mt19937 rng_;
    std::set<size_t> exhausted_;
};

}; // core::fp::detail
