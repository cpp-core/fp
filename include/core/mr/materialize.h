// Copyright (C) 2021 by Mark Melton
//

#pragma once
#include "core/mr/util.h"

namespace mr {

template<Expression, class>
struct Map;

template<class T>
struct Interface {
    template<class F>
    auto transform(F&& function) {
	return Map{std::move(ref()), std::forward<F>(function)};
    }

private:
    auto& ref() { return *static_cast<T*>(this); }
};

template<class C, class O>
struct DataOutput : OutputBase<O> {
    DataOutput(C& data, O&& output)
	: OutputBase<O>(std::forward<O>(output))
	, data_(data) {
    }
    
    void run() {
	for (auto&& elem : data_)
	    this->output_(elem);
    }

private:
    C& data_;
};

template<class C>
struct Data : Interface<Data<C>> {
    using value_type = typename std::decay_t<C>::value_type;
    Data(std::add_rvalue_reference_t<C> data)
	: data_(std::forward<C>(data)) {
    }

    template<class O>
    auto compile(O&& output) {
	return DataOutput{data_, std::forward<O>(output)};
    }
    
private:
    C data_;
};

template<class C> Data(C&&) -> Data<C>;

template<class F, class O>
struct MapOutput : OutputBase<O> {
    MapOutput(F& function, O&& output)
	: OutputBase<O>(std::forward<O>(output))
	, function_(function) {
    }

    void operator()(const auto& elem) {
	this->output_(function_(elem));
    }

private:
    F& function_;
};

template<class F, class O> MapOutput(F&, O&&) -> MapOutput<F,O>;

template<Expression E, class F>
struct Map {
    using value_type = std::result_of_t<F&(expr_value_t<E>)>;
    
    Map(E&& source, F&& function)
	: source_(std::forward<E>(source))
	, function_(std::forward<F>(function)) {
    }

    template<class O>
    auto compile(O&& output) {
	return source_.compile(MapOutput{function_, std::forward<O>(output)});
    }

    E source_;
    F function_;
};

template<class F>
auto transform(F function) {
    return [=]<Expression E>(E&& expr) {
	return Map{std::forward<E>(expr), std::move(function)};
    };
}

template<class T>
struct Vector : public std::vector<T> {
    using Base = std::vector<T>;
    using Base::Base;
    void operator()(auto elem) {
	this->push_back(elem);
    }
};

template<class T, PipeOp Op>
auto operator|(std::vector<T>& data, Op&& op) {
    return op(Data{data});
}

template<Expression E, PipeOp Op>
auto operator|(E&& expr, Op&& op) {
    return op(std::forward<E>(expr));
}

inline auto materialize() {
    return []<Expression E>(E&& expr) {
	Vector<expr_value_t<E>> result;
	auto cexpr = expr.compile(result);
	cexpr.run();
	return result;
    };
}

}; // mr
