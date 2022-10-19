// Copyright (C) 2022 by Mark Melton
//

#pragma once
#include "core/fp/detail/seq/sequences.h"
#include "core/tuple/map.h"
#include "core/tuple/to_array.h"

namespace core::fp::detail {

template<Sequence S, Sequence... Ss>
struct Concat : Interface<Concat<S, Ss...>> {
    using source_type = Sequences<S, Ss...>;
    using value_type = sequence_value_t<S>;
    static constexpr auto N = 1 + sizeof...(Ss);
    static_assert(N <= 10, "Add additional cases more than 10 sequences are needed");
    
    Concat(source_type&& tuple)
	: source_(std::forward<source_type>(tuple))
	, index_(0) {
    }

    std::optional<value_type> next() {
	if (index_ < N) {
	    
#define CHECK_IDX(I)						\
    if constexpr (I < N) {					\
	if (index_ == I) {					\
	    if (auto p = std::get<I>(source_.ref()).next())	\
		return *p;					\
	    else						\
		++index_;					\
	}							\
    }
	    
	    CHECK_IDX(0);
	    CHECK_IDX(1);
	    CHECK_IDX(2);
	    CHECK_IDX(3);
	    CHECK_IDX(4);
	    CHECK_IDX(5);
	    CHECK_IDX(6);
	    CHECK_IDX(7);
	    CHECK_IDX(8);
	    CHECK_IDX(9);
	    
#undef CHECK_IDX

	}
	return std::nullopt;
    }

    source_type source_;
    size_t index_;
};

}; // core::fp::detail
