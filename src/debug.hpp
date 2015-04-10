#pragma once

#include "tuple.hpp"
#include <iostream>

namespace mtl {
namespace debug {

template<typename T>
    void print_typeinfo() {
        std::cout << typeid(T).hash_code() << ": " << typeid(T).name() << std::endl;
    }

} // namespace debug
} // namespace mtl

