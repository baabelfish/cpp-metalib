#include "../src/tuple.hpp"

using A = std::tuple<int, int>;
using B = std::tuple<long, char>;

// Concat
using C = mtl::concat<A, B>;
using D = mtl::concat<A>;
using E = mtl::concat<A, B, C, D>;
static_assert(std::tuple_size<C>::value == 4, "C should be size of 4");
static_assert(std::tuple_size<D>::value == 2, "D should be size of 2");
static_assert(std::tuple_size<E>::value == 10, "E should be size of 10");

// Head
static_assert(std::is_same<long,
                           mtl::head<B>>::value, "");

// Index of type

// Select
static_assert(std::is_same<std::tuple<char, int>,
                           mtl::select<C, 3, 1>>::value, "");

// Tail
static_assert(std::is_same<std::tuple<int, long, char>,
                           mtl::tail<C>>::value, "");

// // Transform
static_assert(std::is_same<std::tuple<int, int>,
                           mtl::transform<B, mtl::transform_to<int>::value>>::value, "");
static_assert(std::is_same<std::tuple<long, char>,
                           mtl::transform<B, mtl::identity>>::value, "");

// Filter
template<typename T>
struct no_ints {
    static constexpr auto value = !std::is_same<T, int>::value;
};
static_assert(std::is_same<std::tuple<char, float>,
                           mtl::filter<no_ints, std::tuple<int, char, int, float>>>::value, "");

// Unique
static_assert(std::is_same<std::tuple<int, char, float, long>,
                           mtl::unique<std::tuple<int, char, int, float, char, long>>>::value, "");

int main() {
    return 0;
}
