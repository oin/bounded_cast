**bounded_cast** is a small header-only library for conversions between numbers of different types and within different bounded ranges.

Read [bounded_cast.hpp](bounded_cast.hpp) for details, and [test.cpp](test.cpp) for an example.

## Basic usage

```c++
auto convertedValueOfTypeB = bounded_cast<TypeB, TypeA>(valueOfTypeA);
```

`TypeA` and `TypeB` can be any arithmetic type (e.g., `long int` or `uint8_t`), in which case the ranges taken into account are between the minimum and maximum value for these types.

However, you may also use tag structures that define a particular bounded range and an arithmetic type.
Some are already defined for convenience:

 - `float01` defines `float` values between 0 and 1
 - `unsigned_int<7>` defines `uint8_t` values between 0 and 127

## Examples

```c++
bounded_cast<uint8_t,float01>(0.5f); // 127
bounded_cast<uint8_t,float11>(0.0f); // 127
bounded_cast<unsigned_int<7>,float01>(1.0f); // 127
bounded_cast<float01,float11>(0.0f); // 0.5
bounded_cast<float01,unsigned_int<12>>(1300); // 0.31746
```

## License

[MIT License](LICENSE.md).
