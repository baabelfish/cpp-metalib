#include "../mtl.hpp"

static_assert(mtl::dec(5) == 4, "");
static_assert(mtl::dec(5) == 4, "");
static_assert(mtl::pow(4) == 16, "");
static_assert(std::is_same<mtl::identity<int>::type, int>::value, "");
static_assert(std::is_same<mtl::transform_to<int>::value<char>::type, int>::value, "");

int main() {
    return 0;
}
