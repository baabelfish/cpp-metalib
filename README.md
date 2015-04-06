cpp-metalib [![Build Status](https://travis-ci.org/baabelfish/cpp-metalib.svg?branch=master)](https://travis-ci.org/baabelfish/cpp-metalib)
===========

Header only metaprogramming library for C++11.

# Tuples

```c++
template<typename T>
struct no_ints {
    static constexpr auto value = !std::is_same<T, int>::value;
};

using Example = std::tuple<int, long, char, long, int, int>;
using A = std::tuple<int, int>;
using B = std::tuple<long>;
using namespace mtl;
```

| Function  | Description                                 | Example |
|-----------|---------------------------------------------|---------|
| has\_type | Check if tuple has a type                   | ``has_type<int, Example>::value == true`` |
| select    | Selects all defined fields from a tuple     | ``select<Example, 2, 1, 0> // std::tuple<char, long, int>`` |
| head      | Selects the first type from a tuple         | ``head<Example> // int`` |
| tail      | All but first from a tuple                  | ``tail<Example> // std::tuple<long, char, long, int, int>`` |
| concat    | Combines N-tuples into one                  | ``concat<A, B> // std::tuple<int, int, long>`` |
| transform | Transforms types in a tuple                 | ``transform<A, identity> // std::tuple<int, int>`` |
| filter    | Filters types from a tuple with a predicate | ``filter<no_ints, Example> // std::tuple<long, char, long>`` |
| unique    | Returns a new tuple with only unique types  | ``unique<Example> // std::tuple<int, long, char>`` |
