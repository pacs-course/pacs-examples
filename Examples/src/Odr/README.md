# ODR Violation Example (Multiple Definition at Link Time)

This folder shows a common C++ mistake related to the **One Definition Rule (ODR)**.

## Files

- `main.cpp` includes `fun1.hpp` and `fun3.hpp`.
- `fun3.cpp` includes `fun1.hpp` and defines `fun3`.
- `fun1.hpp` defines:
  - `fun1(double)` **not inline** (problematic in a header)
  - `fun2(double)` inline (safe in a header)
- `main_fixed.cpp`, `fun3_fixed.cpp`, `fun1_fixed.hpp` provide the corrected variant.

## Why the linker error happens

`fun1.hpp` is included in **multiple translation units** (`main.cpp` and `fun3.cpp`).
Because `fun1(double)` is fully defined in the header and is **not** marked `inline`, each translation unit generates its own external definition of `myspace::fun1(double)`.

At link time, the linker finds multiple strong definitions of the same symbol and reports an error similar to:

```text
multiple definition of `myspace::fun1(double)`
```

This violates the ODR.

## Build with separate compilation and linking

The provided `Makefile` keeps compile and link steps separate:

1. Compile each `.cpp` into `.o`:
   - `main.cpp -> main.o`
   - `fun3.cpp -> fun3.o`
2. Link all object files into one executable.

Commands:

```bash
make
```

This default target intentionally fails at link time to show the ODR problem.

To build the corrected variant:

```bash
make fixed
```

The fixed build outputs `odr_example_fixed`.

To clean generated files:

```bash
make clean
```

## How to avoid this error

Use one of these approaches:

1. Move `fun1` definition to a `.cpp` file and keep only a declaration in the header.
2. If you intentionally define it in the header, mark it `inline`.
3. Use `constexpr` when appropriate (for suitable functions/contexts).

A typical fix for this code is:

```cpp
inline double fun1(double x) {
    return x + 1;
}
```

Then every translation unit can include the header without causing multiple-definition link errors.

# More details: inspecting symbols with `nm --demangle`

`nm` lists symbols stored in object files and executables.
The `--demangle` option converts C++ mangled names into readable function names, so linker messages are easier to interpret.

For this example, you can inspect both object files and see that they both define `myspace::fun1(double)`:

```bash
make clean
g++ -std=c++17 -Wall -Wextra -pedantic -c main.cpp -o main.o
g++ -std=c++17 -Wall -Wextra -pedantic -c fun3.cpp -o fun3.o
nm --demangle main.o | grep 'myspace::fun1'
nm --demangle fun3.o | grep 'myspace::fun1'
```

If both commands print a `T myspace::fun1(double)` entry, it means both translation units provide a strong definition of the same symbol.
That is exactly why the linker reports a multiple-definition error and the ODR is violated.

For the inline function `fun2`, the output is different:

```bash
nm --demangle main.o | grep 'myspace::fun2'
nm --demangle fun3.o | grep 'myspace::fun2'
```

Typical output:

```text
0000000000000000 W myspace::fun2(double)
0000000000000000 W myspace::fun2(double)
```

Here `W` means weak symbol. Weak definitions can appear in multiple object files, and the linker is allowed to merge/select them without reporting a multiple-definition error.
That is why `fun2` (inline) does not trigger the ODR linker failure, while `fun1` (non-inline in a header) does.



