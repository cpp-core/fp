// Copyright (C) 2021 by Mark Melton
//

#pragma once
#include "core/common.h"

namespace core::mr::detail {

template<class O>
struct OutputBase {
    using Output = O;
    OutputBase(O&& output)
	: output_(std::forward<O>(output)) {
    }
    O output_;
};

template<class O> OutputBase(O&&) -> OutputBase<O>;

struct MapReduceExprProto {
    using value_type = int;
    void compile() {}
};

template<class T>
concept OutputExpression = requires (T t) {
    typename T::Output;
};

template<class T>
concept Expression = requires (T t) {
    typename T::value_type;
    t.compile;
};

template<class Op>
concept PipeOp = requires (Op op) {
    std::is_invocable_v<Op, MapReduceExprProto>;
};

template<Expression E>
using expr_value_t = typename E::value_type;

}; // core::mr::detail
