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

template<class T>
concept SequencePredicate = requires(T t) {
    t.operator();
};

template<class T>
concept SequenceTransform = requires(T t) {
    t.operator();
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
