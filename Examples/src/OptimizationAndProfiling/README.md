# Optimization, Debugging, and Profiling Examples

This directory collects small stand-alone examples used to discuss:

- debugging
- assertions
- compiler optimization
- profiling
- memory analysis
- code coverage
- a few common programming mistakes

Some examples are intentionally buggy. That is part of their teaching purpose:
they are meant to be inspected with a debugger, a profiler, or a memory-checking
tool rather than taken as correct production code.

## Prerequisites

Some targets depend on utilities provided elsewhere in the repository.

Before using the examples related to extended assertions, make sure the helper
utilities from `Examples/src/Utilities` are available and built as described in
that folder.

Some targets also require external tools:

- `valgrind` for `leak`, `callgrind`, and `massif`
- `gprof` for `profile`
- `gcov` for `coverage`
- `lcov` and `genhtml` for `lcoverage`
- `kcachegrind` if you want to inspect the output of `callgrind` graphically

## Main Makefile Targets

Running `make` in this folder prints the list of the most useful targets. The
main ones are the following.

### `make esempioExtAss`

Builds two executables showing the use of the extended assertion utilities:

- `esempioExtAss_debug`
- `esempioExtAss_release`

The program reads [`Rational.dat`](./Rational.dat), which contains rational
numbers stored as pairs `numerator denominator`. You can modify the file to
trigger assertion failures, for example by inserting a zero denominator.

### `make optimize`

Compiles and runs three versions of [`esempio.cpp`](./esempio.cpp) with
different optimization levels:

- `esempio0` with `-O0`
- `esempio2` with `-O2`
- `esempio3` with `-O3 -ffast-math -funroll-loops ...`

The program solves a simple one-dimensional heat-transfer problem and writes
its results to `result.dat`. The input file [`dati`](./dati) specifies the
number of elements.

This target is meant to show how optimization flags affect execution time.

### `make debug`

Builds two debug-oriented executables:

- `esempio0_debug`, built from the correct example
- `esempio1_debug`, built from [`esempio_errore.cpp`](./esempio_errore.cpp)

The second program is intentionally wrong: it declares an empty `std::vector`
and then writes into it without resizing it first. This is meant to be
investigated with a debugger.

If you want to inspect crashes through a core dump, see the note at the end of
this README.

### `make leak`

Builds and runs [`leak.cpp`](./leak.cpp) under Valgrind.

This example is intentionally flawed. It shows how returning raw pointers and
forgetting ownership can easily produce memory leaks. The Valgrind report is
written to `leak.log`.

### `make profile`

Builds and profiles [`esempio.cpp`](./esempio.cpp) with `gprof`, both without
optimization and with optimization enabled.

It produces:

- `Profiling_nopt.txt`
- `Profiling_opt.txt`

This target is useful to compare how the profile changes when compiler
optimizations are enabled.

### `make callgrind`

Runs [`esempio.cpp`](./esempio.cpp) under Valgrind's `callgrind` tool with and
without optimization.

It produces:

- `callgrind_O0.out`
- `callgrind_03.out`

These files can be inspected with `kcachegrind`.

### `make massif`

Builds and runs [`provamassif.cpp`](./provamassif.cpp) under Valgrind's
`massif` tool to inspect memory usage over time.

It also creates a text summary:

- `massif.txt`

### `make aliasing`

Builds [`aliasing.cpp`](./aliasing.cpp), a small example showing how argument
aliasing can produce incorrect results.

The code presents several versions of the same operation:

- one that breaks under aliasing
- one that fixes only part of the problem
- one that is safe because it works on copies

This is not a performance example; it is mainly about correctness and API
design.

### `make coverage`

Builds [`esempio.cpp`](./esempio.cpp) with GCC coverage instrumentation and
processes the execution with `gcov`.

This produces coverage data and a text summary in:

- `summary.txt`

It also creates `.gcda`, `.gcno`, and related files.

### `make lcoverage`

Similar to `coverage`, but uses `lcov` and `genhtml` to generate an HTML
coverage report. The output is written in the `html/` directory.

## Important Source Files

- [`esempio.cpp`](./esempio.cpp)
  Main optimization/profiling example. It solves a small numerical problem and
  is used by several targets.

- [`esempio_errore.cpp`](./esempio_errore.cpp)
  Intentionally buggy version used for debugging practice.

- [`esempioExtAss.cpp`](./esempioExtAss.cpp)
  Example of extended assertions applied to rational numbers.

- [`leak.cpp`](./leak.cpp)
  Intentionally faulty memory-management example.

- [`aliasing.cpp`](./aliasing.cpp)
  Example showing the effect of aliasing in function arguments.

- [`provamassif.cpp`](./provamassif.cpp)
  Example used to inspect memory growth with `massif`.

- [`namemangling.cpp`](./namemangling.cpp)
  Small file used to inspect name mangling. It is not tied to a Makefile
  target. Compile it manually, for example:

  ```bash
  g++ -c namemangling.cpp
  nm namemangling.o
  nm --demangle namemangling.o
  ```

- [`rational.hpp`](./rational.hpp), [`rational.cpp`](./rational.cpp)
  Support code used by the extended-assert example.

## What You Learn Here

- how optimization flags can change runtime behavior and performance
- how to debug a crashing program
- how to inspect memory leaks and memory usage
- how to use `gprof`, `callgrind`, `gcov`, and `lcov`
- why some programs are correct but slow, while others are fast but unsafe
- why API choices such as raw pointers or aliased references can be dangerous

## Note on Core Dumps

On many systems, core dumps are disabled by default. The command

```bash
ulimit -c unlimited
```

removes that restriction for the current shell.

On Ubuntu systems this may still not be enough if `apport` is active, because
it intercepts crashes and handles the core dump itself. In that case, if you
want the core file to be written locally for debugging, you may need to stop it
temporarily with:

```bash
sudo systemctl stop apport
```

or:

```bash
sudo service apport stop
```

You can restart it later by replacing `stop` with `start`, or simply reboot the
machine.
