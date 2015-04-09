cpp-metalib [![Build Status](https://travis-ci.org/baabelfish/cpp-metalib.svg?branch=master)](https://travis-ci.org/baabelfish/cpp-metalib)
===========

Header only metaprogramming library for C++11.

# Tuples

```c++
template<typename T>
struct no_ints {
    static constexpr auto value = !std::is_same<T, int>::value; };

using Example = std::tuple<int, long, char, long, int, int>;
using A = std::tuple<int, int>;
using B = std::tuple<long>;
using C = std::tuple<long, char>;
using namespace mtl;
```

| Function (Type really)  | Description                                 | Example |
|-----------|---------------------------------------------|---------|
| has\_type | Check if tuple has a type                   | ``has_type<int, Example>::value == true`` |
| select    | Selects all defined fields from a tuple     | ``select<Example, 2, 1, 0> // std::tuple<char, long, int>`` |
| head      | Selects the first type from a tuple         | ``head<Example> // int`` |
| tail      | All but first from a tuple                  | ``tail<Example> // std::tuple<long, char, long, int, int>`` |
| concat    | Combines N-tuples into one                  | ``concat<A, B> // std::tuple<int, int, long>`` |
| transform | Transforms types in a tuple                 | ``transform<A, identity> // std::tuple<int, int>`` |
| filter    | Filters types from a tuple with a predicate | ``filter<no_ints, Example> // std::tuple<long, char, long>`` |
| unique    | Returns a new tuple with only unique types  | ``unique<Example> // std::tuple<int, long, char>`` |
| without   | Returns tuple without type T                | ``without<int, Example> // std::tuple<long, char, long>`` |
| zip       | Zips two tuples                             | ``zip<A, C> // std::tuple<int, long, int, char>`` |


# Constants
| Function      | Description                                 | Example |
|---------------|---------------------------------------------|---------|
| dec           | constexpr decrement                         | ``dec(5) == 4`` |
| inc           | constexpr increment                         | ``inc(4) == 5`` |
| pow           | constexpr pow                               | ``pow(4) == 16`` |
| identity      | [T] -> [T]                                  | ``identity<char>::type // char`` |
| transform\_to | [G] -> [T]                                  | ``transform_to<char>::value<int>::type // char`` |


# Function traits

```c++
auto my_function = [](int a, int b) {
    return (short)3;
};

using my_function_traits = mtl::function_traits<decltype(my_function)>;
// my_function_traits::return_type <-> short
// my_function_traits::return_type_decayed <-> short
// my_function_traits::arg_types <-> std::tuple<int, int>
// my_function_traits::arg_types_decayed <-> std::tuple<int, int>
// my_function_traits::arg_size <-> 2

```

| Property              | Description                     |
|-----------------------|---------------------------------|
| return\_type          | Return type of the function     |
| return\_type\_decayed | Return type decayed             |
| arg\_types            | Argument types as tuple         |
| arg\_types\_deacyed   | Decayed argument types as tuple |
| arg\_size             | Amount of arguments             |
