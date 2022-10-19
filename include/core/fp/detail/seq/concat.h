// Copyright (C) 2022 by Mark Melton
//

#pragma once
#include "core/fp/detail/seq/sequences.h"
#include "core/tuple/map.h"

namespace core::fp::detail {

template<Sequence S, Sequence... Ss>
struct Concat : Interface<Concat<S, Ss...>> {
    using source_type = Sequences<S, Ss...>;
    using value_type = sequence_value_t<S>;
    static constexpr auto N = 1 + sizeof...(Ss);
    
    Concat(source_type&& tuple)
	: source_(std::forward<source_type>(tuple))
	, index_(0) {
    }

    std::optional<value_type> next() {
	using core::tp::map_nth;
	for (; index_ < N; ++index_)
	    if (auto value = map_nth([](auto& s) { return s.next(); }, index_, source_.ref()))
		return *value;
	return std::nullopt;
    }

    source_type source_;
    size_t index_;
};

}; // core::fp::detail
