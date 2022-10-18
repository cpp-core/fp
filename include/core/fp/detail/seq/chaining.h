// Copyright (C) 2022 by Mark Melton
//

#pragma once
#include "core/fp/detail/seq/sequences.h"

namespace core::fp::chaining {

template<detail::Sequence L, detail::Sequence R>
auto operator*(L&& l, R&& r) {
    return detail::Sequences{std::forward<L>(l), std::forward<R>(r)};
}

template<detail::Sequence... Ss, detail::Sequence S>
auto operator*(detail::Sequences<Ss...>&& source, S &&op) {
    return detail::Sequences{std::tuple_cat(std::move(source.ref()),
					    std::tuple<S>(std::forward<S>(op)))};
}

}; // core::fp::chaining
