**numeric_domain** is a small header-only library for conversions between numbers of different types and within different bounded ranges (called numeric domains here).

Numeric domains can be defined entirely at compile time, or at run time, and a single function `domain_cast` seamlessly converts a value from one domain to another, be they statically or dynamically defined.

Read [numeric_domain.hpp](numeric_domain.hpp) for details, and [test.cpp](test.cpp) for an example.

## Basic usage

Everything is defined under namespace `numeric_domain`.

### Compile-time numeric domains

```c++
auto convertedValueOfTypeB = domain_cast<TypeB, TypeA>(valueOfTypeA);
```

`TypeA` and `TypeB` can be any arithmetic type (e.g., `long int` or `uint8_t`), in which case the ranges taken into account are between the minimum and maximum value for these types.

However, you may also use tag structures that define a particular bounded range and an arithmetic type.
Some are already defined for convenience:

 - `float01` defines `float` values between 0 and 1
 - `unsigned_int<7>` defines `uint8_t` values between 0 and 127

#### Examples

```c++
domain_cast<uint8_t,float01>(0.5f); // 127
domain_cast<uint8_t,float11>(0.0f); // 127
domain_cast<unsigned_int<7>,float01>(1.0f); // 127
domain_cast<float01,float11>(0.0f); // 0.5
domain_cast<float01,unsigned_int<12>>(1300); // 0.31746
```

### Run-time numeric domains

Create instances of class `dynamic_domain<T>` to define run-time numeric domains.
The preferred way is using `make_domain<T>()`:

```c++
auto floatBetween100and200 = make_domain(100.0f, 200.0f);
```

As with compile-time numeric domains, domain conversion takes place using `domain_cast`:

```c++
auto integerBetweenMinus128And128 = make_domain(-128, 128);
auto value = 150.0f;
auto valueBroughtToThatIntegerType = domain_cast(integerBetweenMinus128And128, value, floatBetween100and200);
```

### Wrap-up

`domain_cast` can be used in four ways:

 - between two compile-time numeric domains: `domain_cast<TypeTo, TypeFrom>(value)`
 - between two run-time numeric domains: `domain_cast(domainTo, value, domainFrom)`
 - from a run-time numeric domain to a compile-time one: `domain_cast<TypeTo>(value, domainFrom)`
 - from a compile-time numeric domain to a run-time one: `domain_cast<TypeFrom>(domainTo, value)`

## License

[MIT License](LICENSE.md).
