// Copyright (C) 2022 by Mark Melton
//

#pragma once
#include "core/fp/detail/seq/interface.h"

namespace core::fp::detail {

template<Sequence S>
struct Group : Interface<Group<S>> {
    using value_type = std::vector<sequence_value_t<S>>;

    Group(S&& source, size_t count, bool whole)
	: seq_(std::forward<S>(source))
	, count_(count)
	, whole_(whole) {
    }

    Group(const Group&) = delete;
    Group(Group&&) = default;
    
    std::optional<value_type> next() {
	group_.resize(0);
	while (auto p = seq_.next()) {
	    group_.push_back(*p);
	    if (group_.size() == count_)
		break;
	}
	
	if (group_.size() == count_ or (not whole_ and group_.size() > 0))
	    return group_;
	    
	return std::nullopt;
    }
    
    S seq_;
    size_t count_;
    bool whole_;
    value_type group_;
};

}; // core::fp::detail
