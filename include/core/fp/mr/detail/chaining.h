// Copyright (C) 2021, 2022 by Mark Melton
//

#pragma once
#include "core/fp/mr/detail/source.h"

namespace core::mr {

template<detail::ContiguousContainer C, class Op>
requires std::is_invocable_v<Op, detail::Source<C>>
auto operator|(C&& data, Op&& op) {
    return op(detail::Source{std::forward<C>(data)});
}

template<detail::Expression E, class Op>
requires std::is_invocable_v<Op, E>
auto operator|(E&& expr, Op&& op) {
    return op(std::forward<E>(expr));
}

}; // core::mr
