#include "../mtl.hpp"
#include "../src/debug.hpp"
#include <iostream>

using A = std::tuple<int, int>;
using B = std::tuple<long, char>;

// Has type
// ================================================================================
static_assert(mtl::has_type<char, B>::value, "");
static_assert(mtl::has_type<long, B>::value, "");
static_assert(!mtl::has_type<int, B>::value, "");

// Concat
// ================================================================================
using C = mtl::concat<A, B>;
using D = mtl::concat<A>;
using E = mtl::concat<A, B, C, D>;
static_assert(std::tuple_size<C>::value == 4, "C should be size of 4");
static_assert(std::tuple_size<D>::value == 2, "D should be size of 2");
static_assert(std::tuple_size<E>::value == 10, "E should be size of 10");

// Head
// ================================================================================
static_assert(std::is_same<long,
                           mtl::head<B>>::value, "");

// Index of type

// Select
// ================================================================================
static_assert(std::is_same<std::tuple<char, int>,
                           mtl::select<C, 3, 1>>::value, "");

// Tail
// ================================================================================
static_assert(std::is_same<std::tuple<int, long, char>,
                           mtl::tail<C>>::value, "");

// Transform
// ================================================================================
static_assert(std::is_same<std::tuple<int, int>,
                           mtl::transform<B, mtl::transform_to<int>::value>>::value, "");
static_assert(std::is_same<std::tuple<long, char>,
                           mtl::transform<B, mtl::identity>>::value, "");

// Filter
// ================================================================================
template<typename T>
    struct no_ints {
        static constexpr auto value = !std::is_same<T, int>::value;
    };

static_assert(std::is_same<std::tuple<char, float>,
                           mtl::filter<no_ints, std::tuple<int, char, int, float>>>::value, "");

// Unique
// ================================================================================
static_assert(std::is_same<std::tuple<int, char, float, long>,
                           mtl::unique<std::tuple<int, char, int, float, char, long>>>::value, "");

// Without
// ================================================================================
static_assert(std::is_same<std::tuple<char, float, char, long>,
                           mtl::without<int, std::tuple<int, char, int, float, char, long>>>::value, "");

// Interleave
// ================================================================================
static_assert(std::is_same<std::tuple<int, long, char, long, int, long>,
                           mtl::interleave<std::tuple<int, char, int>, std::tuple<long, long, long>>>::value, "");
static_assert(std::is_same<std::tuple<int, char, char>,
                           mtl::interleave<std::tuple<int>, std::tuple<char, char>>>::value, "");

// Merge
// ================================================================================
using MA = std::tuple<float, int, float>;
using MB = std::tuple<int, char, char>;
using MVec = std::tuple<float, int, char>;

using MC = mtl::merge<MA, MB, MVec>;
static_assert(std::tuple_size<MC>::value == 6, "");
static_assert(std::is_same<std::tuple<float, int, float, int, char, char>, MC>::value, "");

// Sort
// ================================================================================
using ST = std::tuple<int, char, int, int, float, char, float, int>;
using SV = std::tuple<char, int, float>;
using SC = mtl::sort<ST, SV>;
static_assert(std::is_same<std::tuple<char, char, int, int, int, int, float, float>, SC>::value, "");

int main() { return 0; }
