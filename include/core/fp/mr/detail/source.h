// Copyright (C) 2021, 2022 by Mark Melton
//

#pragma once
#include "core/fp/mr/detail/interface.h"

namespace core::mr {

namespace detail {

template<class O, class C>
struct SourceOutput : OutputBase<O> {
    using Iterator = typename C::iterator;
    
    SourceOutput(O&& output, C& data)
	: OutputBase<O>(std::forward<O>(output))
	, data_(data) {
    }

    Iterator begin() {
	return data_.begin();
    }

    Iterator end() {
	return data_.end();
    }

    void operator()(auto elem) {
	this->output_(elem);
    }
    
    void operator()(Iterator begin, Iterator end) {
	for (auto iter = begin; iter != end; ++iter)
	    this->output_(*iter);
    }
    
    auto& run() {
	for (auto&& elem : data_)
	    this->output_(elem);
	return this->output_.result();
    }

    void apply() {
	for (auto&& elem : data_)
	    this->output_(elem);
    }

private:
    C& data_;
};

template <class O, class C> SourceOutput(O&&, C&) -> SourceOutput<O,C>;

template<class C>
struct Source : Interface<Source<C>> {
    using value_type = typename std::decay_t<C>::value_type;
    Source(std::add_rvalue_reference_t<C> data)
	: data_(std::forward<C>(data)) {
    }

    template<class O>
    auto compile(O&& output) {
	return SourceOutput{std::forward<O>(output), data_};
    }
    
private:
    C data_;
};

template<class C> Source(C&&) -> Source<C>;

}; // detail

template<class C>
auto source(C&& data) {
    return detail::Source{std::forward<C>(data)};
}

}; // core::mr
