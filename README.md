# Important Notice

On February 16, 2026, this repository underwent a major restructuring and is
not backward-compatible with earlier clones.

If you cloned the repository before February 16, 2026, clone it again. A
simple `git pull` is not enough for that transition.

# Examples and Exercises for the AMSC and APSC (PACS) Courses

This repository contains source code and documentation for examples and
exercises used in the PACS course (*Programmazione Avanzata per il Calcolo
Scientifico*, Advanced Programming for Scientific Computing) and in the AMSC
course (Advanced Methods for Scientific Computing) at Politecnico di Milano.

For the AMSC course, only a subset of the material is typically used in
lectures and labs, but interested students are encouraged to explore the full
collection.

## Branches

The material for the AMSC course is in the `AMSC` branch, not in `master`.
The `master` branch is the main branch for the APSC/PACS course.

You may of course create your own branches for experiments.

## Cloning the Repository

This repository uses Git submodules, so it should be cloned with:

```bash
git clone --recurse-submodules https://github.com/HPC-Courses/AMSC-CodeExamples.git
```

or, if you use SSH,

```bash
git clone --recurse-submodules git@github.com:HPC-Courses/AMSC-CodeExamples.git
```

In the latter case, your SSH keys must already be registered with GitHub.

To keep your local repository up to date, use:

```bash
git pull --recurse-submodules
```

or, from the root of this repository,

```bash
./update.sh
```

## Compiler Requirements

You need a reasonably recent C++ compiler with solid C++20 support, and some
examples also require C++23 features. In practice, the current configuration in
`Examples/Makefile.user` defaults to `c++23`.

As a rule of thumb, use at least:

- `gcc` 13 or newer, or
- `clang` 18 or newer.

To check your compiler version, run for example:

```bash
g++ -v
```

Compiler support tables are available
[here](https://en.cppreference.com/w/cpp/compiler_support).

## The `mk` Module System

The course environment may use the `mk` module system provided in
[this repository](https://github.com/pcafrica/mk_). This is unrelated to Git
submodules.

Using that module system is convenient, but not strictly required if you have a
recent Linux installation and install the required packages yourself.

The script `load_modules.sh` contains one example of a module setup used in the
course environment.

## IDEs

You may want to use an IDE. Common choices are:

- [Eclipse](https://www.eclipse.org/ide/)
- [CLion](https://www.jetbrains.com/clion/)
- [Visual Studio Code](https://code.visualstudio.com/)

CLion and Visual Studio Code can be integrated with AI-assisted coding tools.

## Repository Contents

At the top level, the most relevant directories and scripts are:

- `Examples/`: the main collection of examples and their build instructions.
- `Extras/`: third-party or auxiliary material used by some examples.
- `shared_files/`: additional shared material.
- `load_modules.sh`: helper script for environments that use the `mk` module
  system.
- `install-git-submodules.sh`: initializes and updates submodules if you did
  not clone with `--recurse-submodules`.
- `change_submodules_url_https.sh`: switches submodule URLs to HTTPS.
- `change_submodules_url_ssh.sh`: switches submodule URLs back to SSH.
- `update.sh`: pulls the repository and its submodules, then runs the
  `Examples/setup.sh` helper.

## Using `load_modules.sh`

If you use the `mk` module system, you may load the main modules with:

```bash
source load_modules.sh
```

or equivalently:

```bash
. ./load_modules.sh
```

This loads the main toolchain and several libraries used by the examples.
`hdf5` is left commented out because it is only needed by a small subset of the
code.

## Installing Git Submodules Later

If you cloned the repository without `--recurse-submodules`, you can initialize
the submodules afterwards with:

```bash
./install-git-submodules.sh
```

The file `submodule_commands.txt` also contains a few useful reminders for
working with submodules.

## What To Do Next

1. Go to `Extras/` and read its local `README.md`.
2. Go to `Examples/` and follow the instructions in its local `README.md`.

## Related Material

If you also want the repository used for the course laboratories, see:

[AMSC-Labs](https://github.com/HPC-Courses/AMSC-Labs)
[PACS-Labs](https://github.com/pacs-course/pacs-Labs)

## Why Makefiles?

The examples are intentionally organized as relatively independent pieces of
software that still share some common infrastructure. In this setting, plain
Makefiles remain simple and practical.

Migrating the repository to CMake is possible, but it is not currently
planned. However, there is an experimental branch, called `cmake` that contains a 
previous version of these examples, ported to cmake. Have a try if you wish.

## Disclaimer

This repository is a collection of teaching examples. Some parts are small and
illustrative; others are more complete and may serve as a starting point for
further work.

You are free to use the code, but please give credit where appropriate and keep
in mind that no warranty is provided regarding correctness, suitability, or
absence of bugs.

Luca Formaggia (`lformaggia` on GitHub)
