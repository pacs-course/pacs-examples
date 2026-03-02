# Numeric limits and numerical constants #

This example program presents the main utilities provided by `std::numeric_limits`. It shows how to inspect key properties of fundamental types, such as machine epsilon and the maximum representable value. It also demonstrates how to query floating-point capabilities, including support for `Inf` and `NaN`. The source code is extensively commented.

The example also introduces the `<numbers>` header, a modern addition to the standard library. This header provides mathematical constants as constant expressions for multiple floating-point types (`double`, `long double`, `float`). Notable constants include pi, Euler's number `e`, the square root of 2, and the golden ratio. Since these values are `constexpr`, expressions that use them can be evaluated at compile time when possible.

### Quiet and non-quiet NaN: what are they? ###

In IEEE-compliant floating-point arithmetic, `NaN` denotes the result of an invalid arithmetic operation. `NaN`s are propagating values: operations involving a `NaN` typically produce a `NaN`. In IEEE-compliant architectures, producing a `NaN` generally does not stop execution. However, the event may still raise a floating-point signal that can be inspected.

Traditionally, `NaN` has also been used to represent **missing data**. For example, unavailable entries in experimental datasets are sometimes encoded as `NaN`. In that case, no arithmetic error has occurred, so signaling behavior is usually undesirable. For this reason, one typically uses a *quiet `NaN`* (also called a *non-signaling `NaN`*). `std::numeric_limits` provides utilities both to generate a quiet `NaN` and to test whether quiet `NaN`s are supported on the target architecture.

**Important note:** In modern C++, missing data should generally be modeled with `std::optional` rather than by using `NaN` as a sentinel value.


**Hint:** To print a floating-point value with the maximum useful precision for its type (here for `double`):
    
    std::cout << std::setprecision(std::numeric_limits<double>::digits10 + 1) << mydouble;
(include `<iomanip>` for `std::setprecision`). The `Utility` folder also contains `scientific_precision.hpp`, which provides a helper to configure an output stream with maximal precision.

**Compilation note:** Building the executable in this directory produces a warning about integer overflow. In production code, this warning often indicates a serious bug.
For signed integers, adding 1 to the maximum representable value may wrap to the minimum representable value, typically without a runtime error. In this example, the warning is intentional because integer overflow behavior is part of what the code demonstrates.

# What do I learn here? #
- A practical set of tools for reasoning about numerical representations. For example, `std::numeric_limits<double>::digits10` gives the number of significant decimal digits in a `double`, while `std::numeric_limits<double>::epsilon()` is analogous to `eps` in MATLAB.
