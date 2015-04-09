#pragma once

#include <utility>

namespace mtl {

template<typename F>
    struct function_traits;

template<typename R, typename... Args>
    struct function_traits<R(Args...)> {
        using return_type = R;
        using return_type_decayed = typename std::decay<R>::type;
        using arg_types = std::tuple<Args...>;
        using arg_types_decayed = std::tuple<std::decay<Args>...>;
        static constexpr auto arg_size = sizeof...(Args);
    };

template<typename T, typename R, typename... Args>
    struct function_traits<R(T::*)(Args...) const> : function_traits<R(Args...)> {};

template<typename Class, typename R, typename... Args>
    struct function_traits<R(Class::*)(Args...)> : public function_traits<R(Args...)> {};

template<typename T>
    struct function_traits : public function_traits<decltype(&T::operator())> {};

} // namespace mtl
