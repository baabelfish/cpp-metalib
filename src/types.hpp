#pragma once

#include <functional>

namespace mtl {

namespace cmp {
template<typename T, typename G>
    struct LessThan {
        static constexpr auto value = true;
    };
};

} // namespace mtl
