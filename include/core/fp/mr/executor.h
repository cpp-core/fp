// Copyright (C) 2017, 2019, 2021, 2022 by Mark Melton
//

#pragma once
#include <thread>
#include "core/fp/mr/detail/executor_base.h"
#include "core/fp/mr/detail/sink.h"
#include "core/mp/same.h"

namespace core::mr {

struct Executor : public detail::ExecutorBase {
    Executor(int number_threads) 
	: ExecutorBase(number_threads) {
    }
    
    Executor() 
	: ExecutorBase() {
    }
    
    size_t chunk_size(size_t max, size_t count) {
	size_t ideal = std::max(1ul, (count - 1) / number_threads());
	return std::min(max, ideal);
    }
    
    template<detail::Expression E>
    requires (not core::mp::is_same_template_v<E, detail::Reduce> and
	      not core::mp::is_same_template_v<E, detail::Apply>)
    auto materialize(E& expr, size_t max_chunk = 4096) {
	using value_type = detail::expr_value_t<E>;
	using Result = std::vector<value_type>;
	
	auto eproto = expr.compile(detail::Sink<value_type>{});
	auto global_begin = eproto.begin();
	auto global_end = eproto.end();
	auto global_count = global_end - global_begin;
	auto chunk = chunk_size(max_chunk, global_count);

	Result global_result{};
	std::atomic counter{0};
	assign_functor([&]() {
	    size_t idx;
	    detail::Sink<value_type> result;
	    auto e = expr.compile(result);
	    while ((idx = counter.fetch_add(chunk)) < global_count) {
		auto begin = global_begin + idx;
		auto end = std::min(begin + chunk, global_end);
		e(begin, end);

		wait_for_sequencer(idx);
		global_result.insert(global_result.end(), result.begin(), result.end());
		advance_sequencer(chunk);
		result.clear();
	    }
	});

	finish_operation();
	return global_result;
    }

    template<detail::Expression E>
    requires (core::mp::is_same_template_v<E, detail::Apply>)
    void materialize(E& expr, size_t max_chunk = 4096) {
	auto eproto = expr.compile();
	auto global_begin = eproto.begin();
	auto global_end = eproto.end();
	auto global_count = global_end - global_begin;
	auto chunk = chunk_size(max_chunk, global_count);

	std::atomic counter{0};
	assign_functor([&]() {
	    size_t idx;
	    while ((idx = counter.fetch_add(chunk)) < global_count) {
		auto begin = global_begin + idx;
		auto end = std::min(begin + chunk, global_end);
		eproto(begin, end);
	    }
	});

	finish_operation();
    }

    template<class E>
    requires core::mp::is_same_template_v<E, detail::Reduce>
    auto materialize(E& expr, size_t max_chunk = 4096) {
	using value_type = detail::expr_value_t<E>;
	using Result = value_type;
	
	auto eproto = expr.compile(detail::Sink<value_type>{});
	auto global_begin = eproto.begin();
	auto global_end = eproto.end();
	auto global_count = global_end - global_begin;
	auto chunk = std::min(max_chunk, 1 + global_count / number_threads());

	auto& reduce = expr.reducer();
	auto& combine = expr.combiner();
	auto& subexpr = expr.source();
	using SubExprValueType = typename std::decay_t<decltype(subexpr)>::value_type;

	Result global_result = expr.initial_value();
	std::atomic counter{0};
	assign_functor([&]() {
	    size_t idx;
	    detail::Sink<SubExprValueType> subexpr_result;
	    auto e = subexpr.compile(subexpr_result);
	    while ((idx = counter.fetch_add(chunk)) < global_count) {
		auto begin = global_begin + idx;
		auto end = std::min(begin + chunk, global_end);
		e(begin, end);
		
		Result r = expr.initial_value();
		for (auto& result : subexpr_result)
		    reduce(r, result);
		subexpr_result.clear();
		
		wait_for_sequencer(idx);
		combine(global_result, r);
		advance_sequencer(chunk);
	    }
	});
	
	finish_operation();
	return global_result;
    }
    
};

namespace detail {

template<Expression E>
struct Parallelize {
    Parallelize(E&& expr, Executor& ex, std::optional<size_t> max_chunk = std::nullopt)
	: expr_(std::forward<E>(expr))
	, ex_(ex)
	, max_chunk_(max_chunk) {
    }

    auto operator()() {
	if (max_chunk_) return ex_.materialize(expr_, *max_chunk_);
	else return ex_.materialize(expr_);
    }

    E expr_;
    Executor& ex_;
    std::optional<size_t> max_chunk_;
};

}; // detail

inline auto eval(Executor& ex) {
    return [&]<detail::Expression E>(E&& expr) {
	return detail::Parallelize{std::forward<E>(expr), ex}();
    };
}

}; // core::mr
