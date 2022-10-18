// Copyright (C) 2022 by Mark Melton
//

#pragma once
#include "core/fp/detail/seq/sequences.h"
#include "core/tuple/map.h"
#include "core/tuple/fold.h"

namespace core::fp::detail {

template<Sequence... Ss>
struct Zip : Interface<Zip<Ss...>> {
    using source_type = Sequences<Ss...>;
    using value_type = std::tuple<sequence_value_t<Ss>...>;

    Zip(source_type&& tuple)
	: source_(std::forward<source_type>(tuple)) {
    }

    std::optional<value_type> next() {
	using core::tp::map_inplace, core::tp::map, core::tp::all;
	auto values = map_inplace([](auto& s) { return s.next(); }, source_.ref());
	if (all(map([](auto& value) { return !!value; }, values)))
	    return map([](auto& value) { return *value; }, values);
	return std::nullopt;
    }

    source_type source_;
};

}; // core::fp::detail
