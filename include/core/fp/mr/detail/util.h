// Copyright (C) 2021, 2022 by Mark Melton
//

#pragma once
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
    typename std::decay_t<T>::value_type;
    t.compile;
};

template<Expression E>
using expr_value_t = typename std::decay_t<E>::value_type;

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

template<class C>
concept Container = requires (C c) {
    typename std::decay_t<C>::value_type;
    c.begin();
    c.end();
};

template<class P, class T>
concept Predicate = requires (P p, T t) {
    requires std::is_invocable_v<std::decay_t<P>, std::decay_t<T>>;
};

template<class F, class T>
concept Transformer = requires (F f, T t) {
    requires std::is_invocable_v<std::decay_t<F>, std::decay_t<T>>;
};

template<class E, class A, class R>
concept Reducer = requires (E e, A a, R r) {
    requires std::is_invocable_v<R, A&, expr_value_t<E>>;
};

template<class E, class A, class C>
concept Combiner = requires (E e, A a, C c) {
    requires std::is_invocable_v<C, A&, A>;
};

template<class F, class T>
concept Applicator = requires (F f, T t) {
    requires std::is_invocable_v<F, std::decay_t<T>&>;
};

}; // core::mr::detail
