// Copyright (C) 2017, 2019, 2021, 2022 by Mark Melton
//

#pragma once
#include <thread>
#include <vector>
#include "core/cc/barrier.h"
#include "core/cc/gate.h"
#include "core/cc/latch.h"
#include "core/cc/sequencer.h"

namespace core::mr::detail {

class ExecutorBase {
public:
    using ThreadIndex = int;
    using TaskId = size_t;
    using Functor = std::function<void()>;

    ExecutorBase(int number_threads = std::thread::hardware_concurrency())
	: ready_latch_(number_threads)
	, finish_barrier_(number_threads, [&](){ work_gate_.close(); }) {
	for (auto i = 0; i < number_threads; ++i)
	    threads_.emplace_back(std::thread{[this]() {
		ready_latch_.arrive_and_wait();
		work_gate_.wait_until_open();
		while (not done_.load()) {
		    functor_();
		    finish_barrier_.arrive_and_wait();
		    work_gate_.wait_until_open();
		}
	    }});
	ready_latch_.wait();
    }

    ~ExecutorBase() {
	terminate();
    }

    size_t number_threads() const {
	return threads_.size();
    }

    template<class F>
    void assign_functor(F&& functor) {
	functor_ = std::forward<F>(functor);
    }

    void wait_for_sequencer(size_t idx) {
	sequencer_.wait_for(idx);
    }

    void advance_sequencer(size_t delta) {
	sequencer_.next(delta);
    }
    
    void finish_operation() {
	sequencer_.reset();
	work_gate_.open_and_wait();
	functor_ = nullptr;
    }

    void terminate() {
	done_ = true;
	work_gate_.open();
	for (auto& thread : threads_)
	    if (thread.joinable())
		thread.join();
	threads_.clear();
    }
    
private:
    std::atomic<bool> done_{false};
    core::cc::Latch ready_latch_;
    core::cc::Barrier finish_barrier_;
    core::cc::Gate work_gate_;
    core::cc::Sequencer sequencer_;
    Functor functor_;
    std::vector<std::thread> threads_;
};

}; // core::mr::detail
