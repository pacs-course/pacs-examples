# C++23: `std::expected`

This workspace contains a small example of returning either a result or an error
using C++23's `std::expected`, declared in `<expected>`.

## Workspace contents

- `main_expected.cpp`: converts strings to integers and demonstrates explicit
  error handling and a fallback value.
- `Makefile`: builds and runs the example, and removes the generated executable.
- `README.md`: explains the example and how to use it.

## Build and run

You need GNU Make and a C++ compiler **and standard library** that support C++23
`std::expected`. The default compiler is `g++`; GCC 13.3 is ok. 

```sh
make          # Build ./main_expected using -std=c++23
make run      # Build if necessary, then run
./main_expected  # Run the executable directly
make clean    # Remove the executable
```

Override the compiler or flags on the command line, for example:

```sh
make clean
make CXX=g++ CXXFLAGS='-O0 -g -Wall -Wextra -Wpedantic'
```

The Makefile also accepts `CPPFLAGS`, `LDFLAGS`, and `LDLIBS`. Clean before
rebuilding with different compiler options, since Make does not track changes
to command-line variables. The C++23 language flag is always added.

The program prints the following two lines, preceded by a blank line (each
output line also has a trailing space):

```text
66 foo: Error -5
66 2023 -5
```

## How `std::expected` works

`std::expected<T, E>` holds either a success value of type `T` or an error of
type `E`. The return type makes the possibility of failure explicit and lets
the caller inspect the error. Unlike `std::optional<T>`, which onnly carries out the information of "not valid data", `std::expected<T,E>` carries information about why an operation failed through the second template argument.

It represent then an easy way to transmit an error condition to the caller without the complexity and overhead of exception handling through try/catch clauses. 

The example defines:

```cpp
std::expected<int, std::string> getInt(std::string arg);
```

`getInt` calls `std::stoi(arg)`. Returning the resulting `int` constructs a
successful `std::expected`. If conversion throws, the catch block returns
`std::unexpected{std::string(arg + ": Error")}`, constructing the error state.
Here, `std::unexpected` is the class template used to mark an error result.

The first loop processes `"66"`, `"foo"`, and `"-5"` by checking each result:

```cpp
auto res = getInt(s);
if (res) {                    // Equivalent to res.has_value()
    std::cout << res.value();  // Read the successful integer
} else {
    std::cout << res.error();  // Read the error message
}
```

Calling `value()` on an error throws `std::bad_expected_access<E>`.
Call `error()` only when the object holds an error; it is not a checked accessor.
Likewise, dereferencing an expected with `*res` requires a success value.

The second loop uses `res.value_or(2023)`: it returns the stored integer on
success, or `2023` on failure. This is convenient when a fallback is acceptable,
but it discards the error information at that call site and cannot distinguish
a failed conversion from a successful conversion of `"2023"`.

## Details of this example

- `std::expected` represents the outcome; it does not automatically catch
  exceptions. This example still uses exceptions internally because `std::stoi`
  throws on invalid input or an out-of-range integer.
- The broad `catch (...)` maps all caught exceptions to the same message. A
  larger application could catch conversion exceptions separately and use an
  error enum or structured error type to preserve their meaning.
- `std::stoi` accepts leading whitespace and a valid numeric prefix. For
  example, `"66abc"` would produce `66` here. To require full consumption, pass
  a position argument to `std::stoi` and check it against the string length.
- Returning an error is not a guarantee that a function cannot throw: creating
  the error string, for example, can allocate memory and throw.

Beyond this example, C++23 also provides `and_then` to chain operations returning
an expected, `transform` to map a success value, `or_else` to handle errors, and
`transform_error` to map an error. These operations require standard-library
support for the monadic extensions (`__cpp_lib_expected >= 202211L`); this
program uses only the basic interface.

## Further reading

- [C++ working draft: expected objects](https://eel.is/c++draft/expected)
- [Original article linked in the source](https://www.modernescpp.com/index.php/c23-a-new-way-of-error-handling-with-stdexpected/)

## What do I learn here?
- The use of an interesting feature intruduced since C++23
