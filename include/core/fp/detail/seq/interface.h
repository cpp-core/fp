// Copyright (C) 2022 by Mark Melton
//

#pragma once
#include "core/fp/detail/seq/util.h"

namespace core::fp::detail {

template<Sequence S>
struct Collect;

template<Sequence S, SequencePredicate P>
struct Filter;

template<Sequence S, class A, class R>
struct Scan;

template<Sequence S>
struct Take;

template<Sequence S, class F>
struct Transform;

template<Sequence... Ss>
struct Zip;

template<class T>
struct Interface {
    template<template <class...> class C>
    auto collect() {
	auto c = Collect{std::move(ref())};
	return c.template run<C>();
    }
    
    template<SequencePredicate P>
    auto filter(P&& predicate) {
	return Filter{std::move(ref()), std::forward<P>(predicate)};
    }

    template<class A, class R>
    auto scan(A&& acc, R&& reducer) {
	return Scan{std::move(ref()), std::forward<A>(acc), std::forward<R>(reducer)};
    }

    auto take(size_t n) {
	return Take{std::move(ref()), n};
    }
    
    template<class F>
    auto transform(F&& function) {
	return Transform{std::move(ref()), std::forward<F>(function)};
    }

    auto zip() {
	return Zip{std::move(ref())};
    }

    auto begin() { return seq_iterator(ref()); }
    auto end() { return sentinel_t{}; }
    
private:
    auto& ref() { return *static_cast<T*>(this); }
};

}; // core::fp::detail
