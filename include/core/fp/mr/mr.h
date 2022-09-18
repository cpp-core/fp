// Copyright (C) 2021, 2022 by Mark Melton
//

#pragma once
#include "core/fp/mr/detail/apply.h"
#include "core/fp/mr/detail/chaining.h"
#include "core/fp/mr/detail/filter.h"
#include "core/fp/mr/detail/interface.h"
#include "core/fp/mr/detail/materialize.h"
#include "core/fp/mr/detail/reduce.h"
#include "core/fp/mr/detail/source.h"
#include "core/fp/mr/detail/transform.h"

// The `mr` namepsace contains the **MapReduce** functionality enabling the construction
// and parallel evaluation of a sequence of processing steps including `apply`, `filter`,
// `map` and `reduce`.
namespace core::mr { };


