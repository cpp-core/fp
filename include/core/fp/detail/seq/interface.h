// Copyright (C) 2022, 2024 by Mark Melton
//

#pragma once
#include "core/fp/detail/seq/util.h"
#include <string>
#include <vector>

namespace core::fp::detail {

template <Sequence S, Sequence... Ss> struct Alternate;

template <Sequence S, class F> struct Apply;

template <Sequence S, Sequence... Ss> struct Choose;

template <Sequence S> struct Collect;

template <Sequence S, Sequence... Ss> struct Concat;

template <Sequence S, class P>
    requires SequencePredicate<S, P>
struct Filter;

template <Sequence S> struct Flatten;

template <Sequence S, class A, class R> struct Fold;

template <Sequence S> struct Group;

template <Sequence S, class A, class R> struct Scan;

template <Sequence S, class F> struct SideEffect;

template <Sequence S> struct Take;

template <Sequence S, class F>
    requires SequenceTransform<S, F>
struct Transform;

template <Sequence S, class F> struct Unique;

template <Sequence S, class A, class F> struct Visit;

template <Sequence... Ss> struct Zip;

template <class T> struct Interface {
    auto alternate() { return Alternate{std::move(ref())}; }

    template <class F> void apply(F &&func) {
        Apply{std::move(ref()), std::forward<F>(func)}.run();
    }

    auto choose() { return Choose{std::move(ref())}; }

    template <template <class...> class C = std::vector> auto collect() {
        auto c = Collect{std::move(ref())};
        return c.template run<C>();
    }

    auto concat() { return Concat{std::move(ref())}; }

    template <class P> auto filter(P &&predicate) {
        static_assert(SequencePredicate<T, P>);
        return Filter{std::move(ref()), std::forward<P>(predicate)};
    }

    auto flatten() { return Flatten{std::move(ref())}; }

    template <class A, class R> auto fold(A &&acc, R &&reducer) {
        auto f = Fold{std::move(ref()), std::forward<A>(acc), std::forward<R>(reducer)};
        return f.run();
    }

    auto group(size_t count, bool whole = false) {
        return Group{std::move(ref()), count, whole};
    }

    auto join(std::string_view sep) {
        auto f = Visit{std::move(ref()), std::string(""), [=](auto &acc, auto value) {
                           if (not acc.empty())
                               acc += sep;
                           acc += value;
                       }};
        return f.run();
    }

    template <class A, class R> auto scan(A &&acc, R &&reducer) {
        return Scan{std::move(ref()), std::forward<A>(acc), std::forward<R>(reducer)};
    }

    template <class F> auto side_effect(F &&function) {
        return SideEffect{std::move(ref()), std::forward<F>(function)};
    }

    auto sum(auto value) {
        return Visit{std::move(ref()), value,
                     [](auto &state, const auto &value) { state += value; }}
            .run();
    }

    auto take(size_t n) { return Take{std::move(ref()), n}; }

    template <class F> auto transform(F &&function) {
        static_assert(SequenceTransform<T, F>);
        return Transform{std::move(ref()), std::forward<F>(function)};
    }

    template <class F> auto unique(F &&function) {
        return Unique{std::move(ref()), std::forward<F>(function)};
    }

    auto unique() {
        return Unique{std::move(ref()), [](const auto &value) { return value; }};
    }

    template <class A, class R> auto visit(A &&acc, R &&reducer) {
        auto f = Visit{std::move(ref()), std::forward<A>(acc), std::forward<R>(reducer)};
        return f.run();
    }

    auto zip() { return Zip{std::move(ref())}; }

    auto begin() { return seq_iterator(ref()); }
    auto end() { return sentinel_t{}; }

private:
    auto &ref() { return *static_cast<T *>(this); }
};

}; // namespace core::fp::detail
