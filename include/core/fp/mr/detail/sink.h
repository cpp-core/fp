// Copyright (C) 2021, 2022 by Mark Melton
//

#pragma once
#include "core/fp/mr/detail/source.h"

namespace core::mr::detail {

template<class T>
struct Sink : public std::vector<T> {
    using Base = std::vector<T>;
    using Base::Base;
    void operator()(auto elem) {
	this->push_back(elem);
    }
    auto& result() {
	return *this;
    }
};

}; // core::mr::detail
