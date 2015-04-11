#pragma once

#include <typeinfo>

namespace mtl {

template<typename T>
    constexpr auto dec(T t) -> T {
        return t - T{1};
    }

template<typename T>
    constexpr auto inc(T t) -> T {
        return t + T{1};
    }

template<typename T>
    constexpr auto pow(T t) -> T {
        return t * t;
    }

template<typename T>
    struct identity {
        using type = T;
    };

template<typename T>
    struct transform_to {
        template<typename G>
            struct value { using type = T; };
    };

} // namespace mtl
