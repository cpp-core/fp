[![Build and Unit Test](https://github.com/cpp-core/fp/actions/workflows/build.yaml/badge.svg)](https://github.com/cpp-core/fp/actions/workflows/build.yaml)

# Functional Programming

Primitives to support a functional programming style.

## At A Glance

* [Either]() -- 
* [Option]() -- 
* [Try]() -- 
* [match]() -- 

## Installation

	git clone https://github.com/cpp-core/fp
	mkdir fp/build && cd fp/build
	CC=clang-mp-14 CXX=clang++-mp-14 cmake -DCMAKE_INSTALL_PREFIX=$HOME/opt -DCORE_FP_TEST=ON ..
	make -j4 check # Run tests
	make install   # Build and install
	
