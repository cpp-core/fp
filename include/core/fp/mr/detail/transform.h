// Copyright (C) 2021, 2022 by Mark Melton
//

#pragma once
#include "core/fp/mr/detail/interface.h"

namespace core::mr {

namespace detail {
		  
template<class O, class F>
struct TransformOutput : OutputBase<O> {
    TransformOutput(O&& output, F& function)
	: OutputBase<O>(std::forward<O>(output))
	, function_(function) {
    }

    void operator()(const auto& elem) {
	this->output_(function_(elem));
    }

private:
    F& function_;
};

template<class O, class F> TransformOutput(O&&, F&) -> TransformOutput<O,F&>;

template<Expression E, class F>
struct Transform : Interface<Transform<E,F>> {
    using value_type = std::result_of_t<F&(expr_value_t<E>)>;
    
    Transform(E&& source, F&& function)
	: source_(std::forward<E>(source))
	, function_(std::forward<F>(function)) {
    }

    template<class O>
    auto compile(O&& output) {
	return source_.compile(TransformOutput{std::forward<O>(output), function_});
    }

    E source_;
    F function_;
};

}; // detail

/// Returns a transform node that applies the given ``function``.
///
template<class F>
auto transform(F&& function) {
    return [function = std::forward<F>(function)]<detail::Expression E>(E&& expr) mutable
	requires detail::Transformer<F, detail::expr_value_t<E>> {
	return detail::Transform{std::forward<E>(expr), std::forward<F>(function)};
    };
}


}; // core::mr
