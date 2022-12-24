// Copyright (C) 2022 by Mark Melton
//

#pragma once
#include <type_traits>
#include <optional>

namespace core::fp::detail {

template<class T>
concept Sequence = requires(T t) {
    typename std::decay_t<T>::value_type;
};

template<class S, class P>
concept SequencePredicate = Sequence<S> and
    std::is_same_v<std::invoke_result_t<P, typename S::value_type>, bool>;

template<class S, class T>
concept SequenceTransform = Sequence<S> and requires {
    std::invoke_result<T, typename S::value_type>();
};

template<Sequence Seq>
using sequence_value_t = typename std::decay_t<Seq>::value_type;

struct sentinel_t {
};

template<class Seq>
struct seq_iterator {
    using value_type = sequence_value_t<std::decay_t<Seq>>;
    using pointer = const value_type*;
    using reference = const value_type&;
    
    seq_iterator(Seq& op)
	: op_(op)
	, p_(op_.next()) {
    }

    reference operator*() const { return *p_; }

    seq_iterator operator++() {
	p_ = op_.next();
	return *this;
    }
    
    friend bool operator!=(const seq_iterator& iter, const sentinel_t&) {
	return !!iter.p_;
    }
    
private:
    Seq& op_;
    std::optional<value_type> p_;
};

}; // core::fp::detail
