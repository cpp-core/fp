// Copyright (C) 2022 by Mark Melton
//

#pragma once
#include "core/fp/detail/seq/interface.h"

namespace core::fp::detail {

template<Sequence... Ss>
struct Sequences : Interface<Sequences<Ss...>> {
    Sequences(Ss&&... seqs)
	: sources_(std::make_tuple(seqs...)) {
    }
    
    Sequences(std::tuple<Ss...>&& sources)
	: sources_(std::forward<std::tuple<Ss...>>(sources)) {
    }

    auto& ref() { return sources_; }
    std::tuple<Ss...> sources_;
};

}; // core::fp::detail

