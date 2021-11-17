// Copyright (C) 2021 by Mark Melton
//

#pragma once
#include "core/mr/detail/source.h"

namespace core::mr {

template<class T, detail::PipeOp Op>
auto operator|(std::vector<T>& data, Op&& op) {
    return op(detail::Source{data});
}

template<detail::Expression E, detail::PipeOp Op>
auto operator|(E&& expr, Op&& op) {
    return op(std::forward<E>(expr));
}

}; // core::mr
