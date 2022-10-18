// Copyright (C) 2022 by Mark Melton
//

#include <iostream>
#include <list>
#include <set>
#include <vector>
#include "core/tuple/map.h"
#include "core/tuple/fold.h"

template<class T>
concept SequenceOp = requires(T t) {
    typename std::decay_t<T>::value_type;
};

template<class T>
concept SequencePredicate = requires(T t) {
    t.operator();
};

template<SequenceOp Op>
using sequence_op_value_t = typename std::decay_t<Op>::value_type;

struct sentinel_t {
};

template<class Op>
struct op_iterator {
    using value_type = sequence_op_value_t<std::decay_t<Op>>;
    using pointer = const value_type*;
    using reference = const value_type&;
    
    op_iterator(Op& op)
	: op_(op)
	, p_(op_.next()) {
    }

    reference operator*() const { return *p_; }

    op_iterator operator++() {
	p_ = op_.next();
	return *this;
    }
    
    friend bool operator!=(const op_iterator& iter, const sentinel_t&) {
	return !!iter.p_;
    }
    
private:
    Op& op_;
    std::optional<value_type> p_;
};

template<SequenceOp Op, SequencePredicate P>
struct Filter;

template<SequenceOp Op, class F>
struct Transform;

template<SequenceOp... Ops>
struct Zip;

/// CRTP
template<class T>
struct Interface {
    template<SequencePredicate P>
    auto filter(P&& predicate) {
	return Filter{std::move(ref()), std::forward<P>(predicate)};
    }

    template<class F>
    auto transform(F&& function) {
	return Transform{std::move(ref()), std::forward<F>(function)};
    }

    auto zip() {
	return Zip{std::move(ref())};
    }
    
private:
    auto& ref() { return *static_cast<T*>(this); }
};

template<SequenceOp E, SequencePredicate P>
struct Filter : Interface<Filter<E,P>> {
    using value_type = sequence_op_value_t<E>;

    Filter(E&& source, P&& predicate)
	: source_(source)
	, predicate_(predicate) {
    }

    auto begin() { return op_iterator{*this}; }
    auto end() { return sentinel_t{}; }
    
    std::optional<value_type> next() {
	while (auto p = source_.next())
	    if (predicate_(*p))
		return *p;
	return std::nullopt;
    }
    
    E source_;
    P predicate_;
};

template<class C>
struct Source : Interface<Source<C>> {
    using value_type = typename std::decay_t<C>::value_type;
    using iter_type = decltype(std::begin(std::declval<C>()));
    Source(std::add_rvalue_reference_t<C> data)
	: data_(std::forward<C>(data))
	, iter_(data_.begin()) {
    }

    auto begin() { return op_iterator{*this}; }
    auto end() { return sentinel_t{}; }
    
    std::optional<value_type> next() {
	if (iter_ == data_.end())
	    return std::nullopt;
	return *iter_++;
    }

private:
    C data_;
    iter_type iter_;
};

template<class C> Source(C&&) -> Source<C>;

template<class C>
auto source(C&& data) {
    return Source{std::forward<C>(data)};
}

template<SequenceOp Op, class F>
struct Transform : Interface<Transform<Op,F>> {
    using input_value_type = sequence_op_value_t<Op>;
    using value_type = std::result_of_t<F&(input_value_type)>;

    Transform(Op&& source, F&& function)
	: source_(std::forward<Op>(source))
	, function_(std::forward<F>(function)) {
    }

    auto begin() { return op_iterator{*this}; }
    auto end() { return sentinel_t{}; }
    
    std::optional<value_type> next() {
	if (auto p = source_.next())
	    return function_(*p);
	return std::nullopt;
    }

    Op source_;
    F function_;
};

template<SequenceOp... Ops>
struct Sequences : Interface<Sequences<Ops...>> {
    Sequences(Ops&&... ops)
	: sources_(std::make_tuple(ops...)) {
    }
    
    Sequences(std::tuple<Ops...>&& sources)
	: sources_(std::forward<std::tuple<Ops...>>(sources)) {
    }

    auto& ref() { return sources_; }
    std::tuple<Ops...> sources_;
};

template<SequenceOp... Ops>
struct Zip : Interface<Zip<Ops...>> {
    using source_type = Sequences<Ops...>;
    using value_type = std::tuple<sequence_op_value_t<Ops>...>;

    Zip(source_type&& tuple)
	: source_(std::forward<source_type>(tuple)) {
    }

    auto begin() { return op_iterator{*this}; }
    auto end() { return sentinel_t{}; }
    
    std::optional<value_type> next() {
	using core::tp::map_inplace, core::tp::map, core::tp::all;
	auto values = map_inplace([](auto& s) { return s.next(); }, source_.ref());
	if (all(map([](auto& value) { return !!value; }, values)))
	    return map([](auto& value) { return *value; }, values);
	return std::nullopt;
    }

    source_type source_;
};

template<SequenceOp L, SequenceOp R>
auto operator*(L&& l, R&& r) {
    return Sequences{std::forward<L>(l), std::forward<R>(r)};
}

template<SequenceOp... Ops, SequenceOp Op>
auto operator*(Sequences<Ops...>&& source, Op &&op) {
    return Sequences{std::tuple_cat(std::move(source.ref()),
				    std::tuple<Op>(std::forward<Op>(op)))};
}

int main(int argc, const char *argv[]) {
    std::vector<int> values{ 1, 2, 3, 4, 5, 6, 7, 8, 9 };

    auto data = source(values)
	.filter([](int n) { return n % 2 == 0; })
	.transform([](int n) { return 2 * n; })
	;
    for (auto n : data)
	std::cout << n << " ";
    std::cout << std::endl;

    auto z = (source(values) * source(values) * source(values))
	.zip();
    for (auto [a, b, c] : z)
	std::cout << a << "," << b << "," << c << "  " ;
    std::cout << std::endl;
    return 0;
}

