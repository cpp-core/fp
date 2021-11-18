// Copyright (C) 2021 by Mark Melton
//

#pragma once
#include "core/mr/detail/interface.h"

namespace core::mr {

namespace detail {
		  
template<class F, class O>
struct TransformOutput : OutputBase<O> {
    TransformOutput(F& function, O&& output)
	: OutputBase<O>(std::forward<O>(output))
	, function_(function) {
    }

    void operator()(const auto& elem) {
	this->output_(function_(elem));
    }

private:
    F& function_;
};

template<class F, class O> TransformOutput(F&, O&&) -> TransformOutput<F,O>;

template<Expression E, class F>
struct Transform : Interface<Transform<E,F>> {
    using value_type = std::result_of_t<F&(expr_value_t<E>)>;
    
    Transform(E&& source, F&& function)
	: source_(std::forward<E>(source))
	, function_(std::forward<F>(function)) {
    }

    template<class O>
    auto compile(O&& output) {
	return source_.compile(TransformOutput{function_, std::forward<O>(output)});
    }

    E source_;
    F function_;
};

}; // detail

template<class F>
auto transform(F&& function) {
    return [function = std::forward<F>(function)]<detail::Expression E>(E&& expr) mutable {
	return detail::Transform{std::forward<E>(expr), std::forward<F>(function)};
    };
}


}; // core::mr
