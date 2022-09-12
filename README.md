[![Build and Unit Test](https://github.com/melton1968/cxx-core-tuple/actions/workflows/build.yaml/badge.svg)](https://github.com/melton1968/cxx-core-tuple/actions/workflows/build.yaml)

# Tuple Operations

Primitives for operating on tuples helping to bridge the gap between
compile-time and run-time.

## At A Glance

* [all]() -- Returns `true` if all members of tuple evaluate to `true`.
* [any]() -- Returns `true` if any members of tuple evaluate to `true`.
* [append]() -- Returns the concatenation of a new member to and existing tuple.
* [apply]() -- Modifies tuple by applying a function to each tuple member
* [car]() -- Returns the first tuple member
* [cdr]() -- Returns a tuple off all but the first tuple member
* [cons]() -- Returns a tuple with a new member prepended
* [compare]() -- Returns a tuple of booleans resulting from comparing two tuples
* [dot product]() -- Returns the sum of products of two tuples
* [find first]() -- Returns index of first member that satisfies a predicate
* [fold_l]() -- Returns result of folding function from left over tuple members
* [fold_r]() -- Returns result of folding function from right over tuple members
* [from array]() -- Creates a tuple from a `std::array` (must be homogeneous)
* [from container]() -- Creates a tuple from a container (must be homogeneous)
* [from vector]() -- Creates a tuple from a vector (must be homogeneous)
* [generate_t]() -- Create a tuple type with `N` homogeneous members
* [map]() -- Returns the new tuple resulting from applying function to each member of tuple.
* [mapply]() -- Returns the new tuple resulting from applying function to each member of tuple.
* [map_n]() -- Returns the new tuple resulting from applying function to each set of members of tuples.
* [max]() -- Returns the maximum member from tuple
* [min]() -- Returns the minimum member from tuple
* [none]() -- Returns `true` if no member of tuple valuate to `true`.
* [parse]() -- Returns a new tuple constructed from parsing an array of strings
* [permute]() -- Returns a new tuple with members permuted according to compile time indices
* [permute_array]() -- Returns a new `std::array` with members permuted according to compile time indices
* [print]() -- Prints a tuple
* [rapply]() -- Apply to a reversed tuple
* [remove]() -- Remove a member from a tuple based on a compile time index.
* [reverse]() -- Returns a new tuple with the order of members reversed
* [select nth]() -- Selection the Nth tuple member
* [select]() -- Select a subset of tuple members
* [scatter]() -- Scatter the members of tuple to a target tuple based on a compile time index
* [to array]() -- Returns a `std::array` containing the tuple members (must be homogeneous)
* [to container]() -- Returns a container containing the tuple members (must be homogeneous)
* [to vector]() -- Returns a vector containing the tuple members (must be homogeneous)

## Installation

### Standalone

	git clone git@github.com:melton1968/cxx-core-tuple
	mkdir cxx-core-pp/build && cd cxx-core-pp/build
    CC=clang-mp-11 CXX=clang++-mp-11 cmake -DCMAKE_INSTALL_PREFIX=$HOME/opt -DCORE_PP_TEST=OFF ..
	make install
	
### Using cxx-depends

    git clone git@github.com:melton1968/cxx-depends
	mkdir cxx-depends/build && cd cxx-depends/build
    CC=clang-mp-11 CXX=clang++-mp-11 cmake -DCMAKE_INSTALL_PREFIX=$HOME/opt ..
	make cxx_core_tuple # Will build and install
