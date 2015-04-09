#include "../mtl.hpp"

struct Test {
    int const_member_function(float a, int b) const {
        return static_cast<int>(a) + b;
    }

    int member_function(float a, int b) {
        return static_cast<int>(a) + b;
    }
};

int free_function(float a, int b) {
    return static_cast<int>(a) + b;
}

auto lambda_function = [](float a, int b) {
    return static_cast<int>(a) + b;
};

static_assert(std::is_same<mtl::function_traits<decltype(free_function)>::arg_types,
                           std::tuple<float, int>>::value, "");
static_assert(std::is_same<mtl::function_traits<decltype(free_function)>::return_type,
                           int>::value, "");

static_assert(std::is_same<mtl::function_traits<decltype(&Test::const_member_function)>::arg_types,
                           std::tuple<float, int>>::value, "");
static_assert(std::is_same<mtl::function_traits<decltype(&Test::const_member_function)>::return_type,
                           int>::value, "");

static_assert(std::is_same<mtl::function_traits<decltype(&Test::member_function)>::arg_types,
                           std::tuple<float, int>>::value, "");
static_assert(std::is_same<mtl::function_traits<decltype(&Test::member_function)>::return_type,
                           int>::value, "");

static_assert(std::is_same<mtl::function_traits<decltype(lambda_function)>::arg_types,
                           std::tuple<float, int>>::value, "");
static_assert(std::is_same<mtl::function_traits<decltype(lambda_function)>::return_type,
                           int>::value, "");
static_assert(mtl::function_traits<decltype(lambda_function)>::arg_size == 2, "");


int main() {
    return 0;
}
