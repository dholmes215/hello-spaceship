# Comparisons in C++

## The old way

### Two "real" comparison operators

C++ has always had six _two-way comparison operators_: `<`, `>`, `<=`, `>=`, `==`, and `!=`.  However all of these can actually be derived from combinations of `<` and `!`, so most algorithms and data structures only require a type to support `<`.

While `==` can also be derived from `<`, in many cases its performance will be poor if implemented that way, so some algorithms will also use `==`.

However, most programmers will be very surprised and frustrated if they find a type where `<` and `==` work but none of the other comparisons do, so typically the remaining operators will be implemented in terms of those two (courtesy of <https://en.cppreference.com/w/cpp/language/operators>):

```cpp
inline bool operator< (const X& lhs, const X& rhs){ /* do actual comparison */ }
inline bool operator> (const X& lhs, const X& rhs){ return rhs < lhs; }
inline bool operator<=(const X& lhs, const X& rhs){ return !(lhs > rhs); }
inline bool operator>=(const X& lhs, const X& rhs){ return !(lhs < rhs); }
```

```cpp
inline bool operator==(const X& lhs, const X& rhs){ /* do actual comparison */ }
inline bool operator!=(const X& lhs, const X& rhs){ return !(lhs == rhs); }
```

### Pre-C++11

TODO: demonstrate the tedious implementation of comparisons before `std::tie`

### Introducing `std::tie`

## Three-way comparisons

## Default comparisons

C++20 adds several features to _automatically_ generate comparison functions for the programmer.

### Default `operator<=>`

The new `operator<=>` can be _explicitly defaulted_:

```cpp
friend auto operator<=>(const T& l, const T& r) noexcept = default;
```

This will cause the compiler to automatically generate a three-way comparison operator, which does a lexicographic comparison of all the class's data members from top to bottom by invoking `<=>` on them.

### Default `==` and `!=`

In C++20, `operator==` can now be defaulted if `==` is valid for all of the class's data members:

```cpp
friend bool operator==(const T& l, const T& r) noexcept = default;
```

This compiler-provided `operator==` will compare each base class and member variable using `==` until one turns up `false`.

Furthermore, in C++20, if `operator==` is defined at all, then `operator!=` will be _implicitly_ defaulted if not explicitly defined or deleted:

```cpp
friend bool operator==(const T& l, const T& r) noexcept;
// Compiler will generate this automatically:
// friend bool operator!=(const T& l, const T& r) noexcept = default;
```

### Default `<`, `>`, `<=`, and `>=`

If `operator<=>` is defined at all, then `<`, `>`, `<=`, `>=`, `==`, and `!=` will _all_ be _implicitly_ defaulted if not explicitly defined:

```cpp
friend auto operator<=>(const T& l, const T& r) noexcept;
// Compiler will create these automatically:
// friend auto operator<(const T& l, const T& r) noexcept = default;
// friend auto operator>(const T& l, const T& r) noexcept = default;
// friend auto operator<=(const T& l, const T& r) noexcept = default;
// friend auto operator>=(const T& l, const T& r) noexcept = default;
// friend auto operator==(const T& l, const T& r) noexcept = default;
// friend auto operator!=(const T& l, const T& r) noexcept = default;
```

If not, the programmer can still explicitly default `<`, `>`, `<=`, and `>=`, and the compiler will attempt to generate functions based on the the `operator<=>` the compiler _would_ generate if it was defaulted:

```cpp
// If this `operator<=>` is _viable_...
// friend auto operator<=>(const T& l, const T& r) noexcept = default;

// ...then the programmer can explicitly default these:
friend auto operator<(const T& l, const T& r) noexcept = default;
friend auto operator>(const T& l, const T& r) noexcept = default;
friend auto operator<=(const T& l, const T& r) noexcept = default;
friend auto operator>=(const T& l, const T& r) noexcept = default;
```

## Equivalence and equality

C++'s `==` relation is typically used to represent _equality_, but it actually only has to test _equivalence_.  Two objects are _equal_ if they represent effectively identical values.  Two objects are _equivalent_ if `a == b` is `true`, which may be the case even if the objects differ in some discernable way.

One illustration of the difference is the floating point `NaN` ("not a number") values.  There are actually many different sequences of bits which are `NaN`, which can be distinguished by reading individual bits out of the value, but the all compare as _equivalent_ to each other when compared with `==`.

Sets of values which are all comparable to each other and for which equivalence is the same as equality are said to have _strong ordering_.  If equivalence may or may not be the same as equality, then the values have _weak ordering_.  If some values can't be compared at all, then they have _partial ordering_.

## Ordering background (math)

## Not one type of ordering, but three

Any `<=>` operator will return an object that's either less than, equal to, or greater than, zero.  However unlike C's `strcmp` or Java's `compareTo`, the returned object is not a number and it communicates more than just the results of three comparisons: it has one of three types and may have more than three values, depending on which _ordering relation_ was chosen

## IEEE floating-point types

IEEE floating-point types will normally TODO

## Standard library header `<compare>`

## Customization point objects

## Under the hood (Compiler Explorer)

## References

* [Comparison operators](https://en.cppreference.com/w/cpp/language/operator_comparison) describes all the comparison operators in detail.
* [Default comparisons](https://en.cppreference.com/w/cpp/language/default_comparisons) contains additional information on defaulted and deleted comparison operators.
* [Operator overloading](https://en.cppreference.com/w/cpp/language/operators) demonstrates how each of the operators is typically overloaded.
* [Standard library header `<compare>`](https://en.cppreference.com/w/cpp/header/compare) lists all the langauge support library APIs required to work with three-way comparisons and orderings in C++20
