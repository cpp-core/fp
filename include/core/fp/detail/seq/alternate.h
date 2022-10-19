// Copyright (C) 2022 by Mark Melton
//

#pragma once
#include "core/fp/detail/seq/sequences.h"
#include "core/tuple/map.h"
#include "core/tuple/to_array.h"

namespace core::fp::detail {

template<Sequence S, Sequence... Ss>
struct Alternate : Interface<Alternate<S, Ss...>> {
    using source_type = Sequences<S, Ss...>;
    using value_type = sequence_value_t<S>;
    static constexpr auto N = 1 + sizeof...(Ss);

    Alternate(source_type&& tuple)
	: source_(std::forward<source_type>(tuple))
	, index_(0) {
    }

    std::optional<value_type> next() {
	for (auto i = 0; i < N; ++i) {
	    if (index_ % N == 0) {
		using core::tp::map_inplace, core::tp::to_array;
		values_ = to_array(map_inplace([](auto& s) { return s.next(); }, source_.ref()));
	    }
	    auto idx = index_++ % N;
	    if (values_[idx])
		return *values_[idx];
	}
	return std::nullopt;
    }

    source_type source_;
    size_t index_;
    std::array<std::optional<value_type>, 1 + sizeof...(Ss)> values_;
};

}; // core::fp::detail
