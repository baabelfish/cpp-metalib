#pragma once

#include <tuple>
#include <limits>
#include <utility>

namespace mtl {

namespace internal {
    static constexpr std::size_t Max = std::numeric_limits<std::size_t>::max();

    template<typename Target, class Tuple> struct index_of_type;
    template<typename Target>
        struct index_of_type<Target, std::tuple<>> {
            static constexpr auto value = Max;
        };
    template<typename Target, typename... Rest>
        struct index_of_type<Target, std::tuple<Target, Rest...>> {
            static constexpr auto value = 0;
        };
    template<typename Target, typename First, typename... Rest>
        struct index_of_type<Target, std::tuple<First, Rest...>> {
            static constexpr auto next = index_of_type<Target, std::tuple<Rest...>>::value;
            static constexpr auto value = next == Max ? Max : next + 1;
        };

    template <typename Target, class Tuple, std::size_t From = 0>
        struct has_type {
            static constexpr auto value = internal::index_of_type<Target, Tuple>::value != internal::Max;
        };

    template<typename Tuple> struct TailHelper;
    template<typename First, typename... Params>
        struct TailHelper<std::tuple<First, Params...>> {
            using type = std::tuple<Params...>;
        };

    template<typename Tuple, template<typename> class Operation> struct TransformHelper;
    template<template<typename> class Operation, typename... Params>
        struct TransformHelper<std::tuple<Params...>, Operation> {
            using type = std::tuple<typename Operation<Params>::type...>;
        };

    template<template<typename> class Predicate, typename Tuple, typename Result, typename Enable = void> struct FilterHelper;
    template<template<typename> class Predicate, typename... Done>
        struct FilterHelper<Predicate, std::tuple<>, std::tuple<Done...>> {
            using type = std::tuple<Done...>;
        };
    template<template<typename> class Predicate, typename Head, typename... Tail, typename... Done>
        struct FilterHelper<Predicate, std::tuple<Head, Tail...>, std::tuple<Done...>,
            typename std::enable_if<!Predicate<Head>::value>::type> {
            using type = typename FilterHelper<Predicate, std::tuple<Tail...>, std::tuple<Done...>>::type;
        };
    template<template<typename> class Predicate, typename Head, typename... Tail, typename... Done>
        struct FilterHelper<Predicate, std::tuple<Head, Tail...>, std::tuple<Done...>,
            typename std::enable_if<Predicate<Head>::value>::type> {
            using type = typename FilterHelper<Predicate, std::tuple<Tail...>, std::tuple<Done..., Head>>::type;
        };

    template<typename Tuple, typename Result, typename Enabled = void> struct UniqueHelper;
    template<typename... Result>
        struct UniqueHelper<std::tuple<>, std::tuple<Result...>> {
            using type = std::tuple<Result...>;
        };
    template<typename... Result, typename Head, typename... Rest>
        struct UniqueHelper<std::tuple<Head, Rest...>, std::tuple<Result...>,
            typename std::enable_if<has_type<Head, std::tuple<Result...>>::value>::type> {
            using type = typename UniqueHelper<std::tuple<Rest...>, std::tuple<Result...>>::type;
        };
    template<typename... Result, typename Head, typename... Rest>
        struct UniqueHelper<std::tuple<Head, Rest...>, std::tuple<Result...>,
            typename std::enable_if<!has_type<Head, std::tuple<Result...>>::value>::type> {
            using type = typename UniqueHelper<std::tuple<Rest...>, std::tuple<Result..., Head>>::type;
        };

} // namespace internal

template<typename T>
    struct transform_to {
        template<typename G>
            struct value { using type = T; };
    };

template<typename T>
    struct identity {
        using type = T;
    };

template<std::size_t X>
    struct dec { static constexpr auto value = X - 1; };

template<std::size_t X>
    struct inc { static constexpr auto value = X + 1; };

template <typename Target, class Tuple, std::size_t From = 0>
    using has_type = typename internal::has_type<Target, Tuple, From>;

template<typename Tuple, std::size_t... Idx>
    using select = std::tuple<typename std::tuple_element<Idx, Tuple>::type...>;

template<typename Tuple>
    using head = typename std::tuple_element<0, Tuple>::type;

template<typename Tuple>
    using tail = typename internal::TailHelper<Tuple>::type;

// template<typename... Tuples>
//     using concat = decltype(std::tuple_cat(Tuples()...));

template<typename Tuple, template<typename> class Operation>
    using transform = typename internal::TransformHelper<Tuple, Operation>::type;

template<template<typename> class Predicate, typename Tuple>
    using filter = typename internal::FilterHelper<Predicate, Tuple, std::tuple<>>::type;

template<typename Tuple>
    using unique = typename internal::UniqueHelper<Tuple, std::tuple<>>::type;

} // namespace mtl
