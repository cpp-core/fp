[![Build and Unit Test](https://github.com/cpp-core/fp/actions/workflows/build.yaml/badge.svg)](https://github.com/cpp-core/fp/actions/workflows/build.yaml)

# Functional Programming

Primitives to support a functional programming style.

## At A Glance

* [Either]() -- 
* [Option]() -- 
* [Try]() -- 
* [match]() -- 

## Installation

    git clone https://github.com/cpp-core/depends
	mkdir depends/build && cd depends/build
    CC=clang-mp-11 CXX=clang++-mp-11 cmake -DCMAKE_INSTALL_PREFIX=$HOME/opt -DCORE_FP_TEST=ON ..
	make fp-check # Run tests
	make fp       # Build and install
	
