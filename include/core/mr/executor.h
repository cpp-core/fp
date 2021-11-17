// Copyright (C) 2017, 2019, 2021 by Mark Melton
//

#pragma once
#include <thread>
#include "core/mr/detail/executor_base.h"
#include "core/mr/detail/sink.h"
#include "core/mp/same.h"
#include "core/utility/type_name.h"

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

    // template<class E>
    // requires core::mp::is_same_template_v<E, detail::Reduce>
    // auto materialize(E& expr, size_t max_chunk = 4096) {
    // 	using Expr = std::decay_t<E>;
    // 	using Result = typename Expr::Result;
    // 	using SubExpr = typename Expr::Source;
    // 	using SubExprResult = typename SubExpr::Result;

    // 	auto& reduce = expr.op();
    // 	auto& combine = expr.combine();
    // 	auto eproto = expr.compile(fp::detail::NullSink{});
    // 	auto global_begin = eproto.data().begin();
    // 	auto global_end = eproto.data().end();
    // 	auto global_count = global_end - global_begin;
    // 	auto chunk = std::min(max_chunk, 1 + global_count / number_threads());

    // 	Result global_result = expr.initial_value();
    // 	auto& subexpr = expr.child();
    // 	std::atomic counter{0};
    // 	if (chunk == 1) {
    // 	    assign_functor([&]() {
    // 		size_t idx;
    // 		SubExprResult subexpr_result{};
    // 		auto e = subexpr.compile(fp::detail::Sink{subexpr_result});
    // 		while ((idx = counter.fetch_add(1)) < global_count) {
    // 		    e(*(global_begin + idx));
    // 		    wait_for_sequencer(idx);
    // 		    for (auto& result : subexpr_result)
    // 			reduce(global_result, result);
    // 		    advance_sequencer(1);
    // 		    subexpr_result.clear();
    // 		}
    // 	    });
    // 	}
    // 	else {
    // 	    assign_functor([&]() {
    // 		size_t idx;
    // 		SubExprResult subexpr_result{};
    // 		auto e = subexpr.compile(fp::detail::Sink{subexpr_result});
    // 		while ((idx = counter.fetch_add(chunk)) < global_count) {
    // 		    auto begin = global_begin + idx;
    // 		    auto end = std::min(begin + chunk, global_end);
    // 		    for (auto iter = begin; iter < end; ++iter)
    // 			e(*iter);

    // 		    Result r = expr.initial_value();
    // 		    for (auto& result : subexpr_result)
    // 			reduce(r, result);
		    
    // 		    wait_for_sequencer(idx);
    // 		    combine(global_result, r);
    // 		    advance_sequencer(chunk);
    // 		    subexpr_result.clear();
    // 		}
    // 	    });
    // 	}
	
    // 	finish_operation();
    // 	return global_result;
    // }
    
};

namespace detail {

template<Expression E>
struct Parallelize {
    Parallelize(E&& expr, Executor& ex)
	: expr_(std::forward<E>(expr))
	, ex_(ex) {
    }

    auto operator()() {
	return ex_.materialize(expr_);
    }

    E expr_;
    Executor& ex_;
};

}; // detail

inline auto eval(Executor& ex) {
    return [&]<detail::Expression E>(E&& expr) {
	return detail::Parallelize{std::forward<E>(expr), ex}();
    };
}

}; // core::mr
