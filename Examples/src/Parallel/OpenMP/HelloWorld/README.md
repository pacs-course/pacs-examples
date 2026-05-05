# OpenMP Hello World

This folder contains two very small programs introducing the basic structure of
an OpenMP application.

## `main_hello_world_simple.cpp`

This is the minimal example:

- the program first runs in serial
- then a `#pragma omp parallel` region is entered
- each thread prints a message

The order of the output is not deterministic, which is normal in a parallel
program.

## `main_hello_world_advanced.cpp`

This version adds a few more OpenMP constructs:

- `num_threads`
- `private` and `shared`
- `critical`
- `barrier`
- `single`
- `omp_get_thread_num()`
- `omp_get_num_threads()`

It is still a toy example, but it is a good place to see the syntax and the
effect of the main OpenMP directives.

## What You Learn Here

- how a parallel region is created
- how variables can be private or shared
- how to serialize a small block with `critical`
- how to synchronize threads with `barrier`
