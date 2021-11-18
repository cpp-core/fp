// Copyright (C) 2021 by Mark Melton
//

#pragma once
#include "core/common.h"
#include "core/mp/same.h"

namespace core::mr::detail {

template<class O>
struct OutputBase {
    using Output = O;
    OutputBase(O&& output)
	: output_(std::forward<O>(output)) {
    }

    auto& result() {
	return output_.result();
    }
    
    O output_;
};

template<class O> OutputBase(O&&) -> OutputBase<O>;

template<class T>
concept OutputExpression = requires (T t) {
    typename T::Output;
};

template<class T>
concept Expression = requires (T t) {
    typename T::value_type;
    t.compile;
};

template<Expression E>
using expr_value_t = typename E::value_type;

template<class I>
concept ContiguousIterator = requires (I iter) {
    requires std::is_same_v<typename std::iterator_traits<std::decay_t<I>>::iterator_category,
    std::random_access_iterator_tag>;
};

template<class C>
concept ContiguousContainer = requires (C c) {
    requires ContiguousIterator<typename std::decay_t<C>::iterator>;
    typename std::decay_t<C>::value_type;
    c.begin();
    c.end();
};

}; // core::mr::detail
