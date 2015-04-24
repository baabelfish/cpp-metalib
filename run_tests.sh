#!/bin/bash
clang++ --version
clang++ -std=c++11 test/tuple.cpp && ./a.out &&
clang++ -Wall -Werror -std=c++11 test/values.cpp &&
clang++ -std=c++11 test/function_traits.cpp
