// Copyright (C) 2022 by Mark Melton
//

#pragma once
#include "core/fp/detail/seq/sequences.h"
#include "core/tuple/map.h"

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

    Alternate(const Alternate&) = delete;
    Alternate(Alternate&&) = default;

    std::optional<value_type> next() {
	using core::tp::map_nth;
	for (auto i = 0; i < N; ++i) 
	    if (auto v = map_nth([](auto& s) { return s.next(); }, index_++ % N, source_.ref()))
		return *v;
	return std::nullopt;
    }

    source_type source_;
    size_t index_;
};

}; // core::fp::detail
