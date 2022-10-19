// Copyright (C) 2022 by Mark Melton
//

#pragma once
#include "core/fp/detail/seq/interface.h"

namespace core::fp::detail {

template<Sequence S>
struct Collect : Interface<Collect<S>> {
    using value_type = sequence_value_t<S>;

    Collect(S&& seq)
	: seq_(std::forward<S>(seq)) {
    }

    Collect(const Collect&) = delete;
    Collect(Collect&&) = default;
    
    template<template<class...> class C>
    auto run() {
	C<value_type> data;
	for (const auto& value : seq_)
	    data.push_back(value);
	return data;
    }

    S seq_;
};

}; // core::fp::detail
