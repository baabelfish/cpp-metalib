#pragma once

#include <tuple>
#include <typeinfo>
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

    template<typename Result, typename... Tuples> struct ConcatImpl;
    template<typename... Result, typename... Types>
        struct ConcatImpl<std::tuple<Result...>, std::tuple<Types...>> {
            using type = std::tuple<Result..., Types...>;
        };
    template<typename... Result, typename... Types, typename... Tuples>
        struct ConcatImpl<std::tuple<Result...>, std::tuple<Types...>, Tuples...> {
            using type = typename ConcatImpl<std::tuple<Result..., Types...>, Tuples...>::type;
        };

    template<typename Tuple> struct TailImpl;
    template<typename First, typename... Params>
        struct TailImpl<std::tuple<First, Params...>> {
            using type = std::tuple<Params...>;
        };

    template<typename Tuple, template<typename> class Operation> struct TransformImpl;
    template<template<typename> class Operation, typename... Params>
        struct TransformImpl<std::tuple<Params...>, Operation> {
            using type = std::tuple<typename Operation<Params>::type...>;
        };

    template<template<typename> class Predicate, typename Tuple, typename Result = std::tuple<>, typename Enable = void> struct FilterImpl;
    template<template<typename> class Predicate, typename... Done>
        struct FilterImpl<Predicate, std::tuple<>, std::tuple<Done...>> {
            using type = std::tuple<Done...>;
        };
    template<template<typename> class Predicate, typename Head, typename... Tail, typename... Done>
        struct FilterImpl<Predicate, std::tuple<Head, Tail...>, std::tuple<Done...>,
            typename std::enable_if<!Predicate<Head>::value>::type> {
            using type = typename FilterImpl<Predicate, std::tuple<Tail...>, std::tuple<Done...>>::type;
        };
    template<template<typename> class Predicate, typename Head, typename... Tail, typename... Done>
        struct FilterImpl<Predicate, std::tuple<Head, Tail...>, std::tuple<Done...>,
            typename std::enable_if<Predicate<Head>::value>::type> {
            using type = typename FilterImpl<Predicate, std::tuple<Tail...>, std::tuple<Done..., Head>>::type;
        };

    template<typename Tuple, typename Result = std::tuple<>, typename Enabled = void> struct UniqueImpl;
    template<typename... Result>
        struct UniqueImpl<std::tuple<>, std::tuple<Result...>> {
            using type = std::tuple<Result...>;
        };
    template<typename... Result, typename Head, typename... Rest>
        struct UniqueImpl<std::tuple<Head, Rest...>, std::tuple<Result...>,
            typename std::enable_if<has_type<Head, std::tuple<Result...>>::value>::type> {
            using type = typename UniqueImpl<std::tuple<Rest...>, std::tuple<Result...>>::type;
        };
    template<typename... Result, typename Head, typename... Rest>
        struct UniqueImpl<std::tuple<Head, Rest...>, std::tuple<Result...>,
            typename std::enable_if<!has_type<Head, std::tuple<Result...>>::value>::type> {
            using type = typename UniqueImpl<std::tuple<Rest...>, std::tuple<Result..., Head>>::type;
        };

    template<typename T>
        struct WithoutFilter {
            template<typename G>
                struct Predicate {
                    static constexpr auto value = !std::is_same<T, G>::value;
                };
        };
    template<typename T, typename Tuple> struct WithoutImpl;
    template<typename T, typename... Params>
        struct WithoutImpl<T, std::tuple<Params...>> {
            using type = typename FilterImpl<WithoutFilter<T>::template Predicate, std::tuple<Params...>, std::tuple<>>::type;
        };

    template<typename A, typename B, typename Result = std::tuple<>> struct InterleaveImpl;
    template<typename... Result>
        struct InterleaveImpl<std::tuple<>, std::tuple<>, std::tuple<Result...>> {
            using type = std::tuple<Result...>;
        };
    template<typename... AParams, typename... Result>
        struct InterleaveImpl<std::tuple<AParams...>, std::tuple<>, std::tuple<Result...>> {
            using type = std::tuple<Result..., AParams...>;
        };
    template<typename... BParams, typename... Result>
        struct InterleaveImpl<std::tuple<>, std::tuple<BParams...>, std::tuple<Result...>> {
            using type = std::tuple<Result..., BParams...>;
        };
    template<typename AFirst, typename... AParams, typename BFirst, typename... BParams, typename... Result>
        struct InterleaveImpl<std::tuple<AFirst, AParams...>, std::tuple<BFirst, BParams...>, std::tuple<Result...>> {
            using type = typename InterleaveImpl<std::tuple<AParams...>, std::tuple<BParams...>, std::tuple<Result..., AFirst, BFirst>>::type;
        };

    template<typename A, typename B, typename Comparison, typename Result = std::tuple<>, typename Enabled = void> struct MergeImpl {};
    template<typename... Right, typename... ResultArgs, typename... CompArgs>
        struct MergeImpl<std::tuple<>, std::tuple<Right...>, std::tuple<CompArgs...>, std::tuple<ResultArgs...>> {
            using type = std::tuple<ResultArgs..., Right...>;
        };
    template<typename... Left, typename... ResultArgs, typename... CompArgs>
        struct MergeImpl<std::tuple<Left...>, std::tuple<>, std::tuple<CompArgs...>, std::tuple<ResultArgs...>> {
            using type = std::tuple<ResultArgs..., Left...>;
        };
    template<typename LeftHead, typename RightHead, typename... Left, typename... Right, typename... CompArgs, typename... ResultArgs>
        struct MergeImpl<std::tuple<LeftHead, Left...>, std::tuple<RightHead, Right...>, std::tuple<CompArgs...>, std::tuple<ResultArgs...>,
                typename std::enable_if<index_of_type<RightHead, std::tuple<CompArgs...>>::value < index_of_type<LeftHead, std::tuple<CompArgs...>>::value>::type> {
            using type = typename MergeImpl<std::tuple<LeftHead, Left...>, std::tuple<Right...>, std::tuple<CompArgs...>, std::tuple<ResultArgs..., RightHead>>::type;
        };
    template<typename LeftHead, typename RightHead, typename... Left, typename... Right, typename... CompArgs, typename... ResultArgs>
        struct MergeImpl<std::tuple<LeftHead, Left...>, std::tuple<RightHead, Right...>, std::tuple<CompArgs...>, std::tuple<ResultArgs...>,
                typename std::enable_if<index_of_type<LeftHead, std::tuple<CompArgs...>>::value
                                <= index_of_type<RightHead, std::tuple<CompArgs...>>::value>::type> {
            using type = typename MergeImpl<std::tuple<Left...>, std::tuple<RightHead, Right...>, std::tuple<CompArgs...>, std::tuple<ResultArgs..., LeftHead>>::type;
        };


} // namespace internal

template<typename Target, class Tuple, std::size_t From = 0> using has_type = typename internal::has_type<Target, Tuple, From>;
template<typename Tuple, std::size_t... Idx> using select = std::tuple<typename std::tuple_element<Idx, Tuple>::type...>;
template<typename Tuple> using head = typename std::tuple_element<0, Tuple>::type;
template<typename Tuple> using tail = typename internal::TailImpl<Tuple>::type;
template<typename... Tuples> using concat = typename internal::ConcatImpl<std::tuple<>, Tuples...>::type;
template<typename Tuple, template<typename> class Operation> using transform = typename internal::TransformImpl<Tuple, Operation>::type;
template<template<typename> class Predicate, typename Tuple> using filter = typename internal::FilterImpl<Predicate, Tuple>::type;
template<typename Tuple> using unique = typename internal::UniqueImpl<Tuple>::type;
template<typename T, typename Tuple> using without = typename internal::WithoutImpl<T, Tuple>::type;
template<typename A, typename B> using interleave = typename internal::InterleaveImpl<A, B>::type;
template<typename A, typename B, typename Comparison> using merge = typename internal::MergeImpl<A, B, unique<Comparison>>::type;

} // namespace mtl
