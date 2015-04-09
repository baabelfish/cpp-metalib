#!/bin/bash
clang++ --version
clang++ -Wall -Werror -std=c++11 test/tuple.cpp &&
clang++ -Wall -Werror -std=c++11 test/values.cpp &&
clang++ -std=c++11 test/function_traits.cpp
