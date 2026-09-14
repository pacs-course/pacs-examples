# Multicity epidemic model

This folder contains a C++ teaching example for simulating epidemic spread
between cities connected by travel. It combines a population mobility model
with susceptible–infectious disease dynamics and integrates both systems of
ordinary differential equations using an adaptive Runge–Kutta solver. The
implementation uses Eigen matrices and templates, with the number of cities
chosen at compile time. Most model classes live in `apsc::multicity`.

The source comments identify *A multi-city epidemic model* by J. Arino and
P. van den Driessche as the basis for the model and example parameters. The
code extends the SIS recovery mechanism with a configurable fraction of
recovered individuals who become immune.

## Model scope and state variables

For `n` cities, the population is an `n × n` matrix `N`. Entry `N(i,j)` counts
residents of city `i` who are currently in city `j`. Diagonal entries represent
people at home; off-diagonal entries represent visitors. Consequently:

- `Nr(N)` gives the resident population of each city (row sums).
- `Np(N)` gives the population physically present in each city (column sums).
- `N.sum()` gives the total population across all cities.

Susceptible (`S`) and infectious (`I`) individuals use the same home/location
indexing. Their two `n × n` matrices are stacked vertically into one `2n × n`
epidemic state, with `S` above `I`.

Residents leave their home city, visit other cities, and return home. Infection
depends on contacts with infectious people at the current location, normalized
by the population present there. The equations also include recovery and
natural mortality, with compensating recruitment into the susceptible
population at home.

The data interface exposes the following coefficients; its methods accept time
so that a different data class can supply time-dependent parameters.

| Coefficient | Role |
| --- | --- |
| `g(t)` | Per-capita departure rates from home cities. |
| `m(t)` | Destination fractions; `m(j,i)` describes movement from home city `i` to city `j`. Supply zero diagonal entries; the model constructs the departure loss terms internally. |
| `r(t)` | Return rates; `r(i,j)` applies to residents of `i` visiting `j`. |
| `k(t)` | Contact-rate factors for each location. |
| `beta(t,q)` | Transmission coefficient matrix used for contacts with infectious residents of home city `q`, across susceptible home cities and current locations. |
| `gamma(t)` | Recovery rate. |
| `d(t)` | Natural mortality rate, also used for compensating recruitment. |
| `immuneFraction(t)` | Fraction of recoveries that confer immunity. |

With `immuneFraction = 0`, recovered individuals become susceptible again
(SIS). With `immuneFraction = 1`, all recovered individuals leave the tracked
`S` and `I` compartments (SIR-style immunity). Intermediate values split
recoveries between these outcomes. There is no separately integrated recovered
matrix; `N - S - I` represents the implied remaining compartment, subject to
numerical consistency of the separately advanced systems.

This is a deterministic compartment model with prescribed mobility and
transmission parameters. The example demonstrates model composition, matrix
state storage, and numerical integration; it does not include parameter fitting,
individual agents, or stochastic transmission.

## Sources and headers

| File | Purpose |
| --- | --- |
| [`main_test.cpp`](main_test.cpp) | Complete two-city demonstration: initializes populations and infections, sets coefficients, computes reproduction-number diagnostics, advances the model, and writes results. |
| [`MultiCitiesModelTraits.hpp`](MultiCitiesModelTraits.hpp) | Defines fixed-size Eigen types for populations, epidemic states, vectors, and right-hand-side functions. Provides `join` and `extract` to assemble and access compartment blocks. |
| [`MultiCityPopulation.hpp`](MultiCityPopulation.hpp) | Population type aliases, `Np`, `Nr`, and `totalPop` helpers, plus the population initialization interface and `initialize2Cities` example. |
| [`MultiCityEpidemic.hpp`](MultiCityEpidemic.hpp) | Epidemic state aliases, the `MultiCityEpidemicVariablesSISProxy` accessors for `S` and `I`, and `MultiCityDataFixed`, which stores constant model coefficients. Implements `initializeFromArticle()` for two cities. |
| [`PopulationModel.hpp`](PopulationModel.hpp) | Population ODE right-hand side, including departures, returns, mortality, and recruitment. Also provides `advanceMultiCityPopulation` for integrating population dynamics alone. |
| [`EpidemicModelSIR.hpp`](EpidemicModelSIR.hpp) | Epidemic ODE right-hand side for `S` and `I`, including mobility, transmission, recovery, and the immunity extension. Holds a reference to the population used in its calculations. |
| [`MulticityModel.hpp`](MulticityModel.hpp) | `MultiCityModelAdvance` couples the population and epidemic solvers. Defines solver settings, the `Result` time histories and success flag, logging, and result serialization. |
| [`MultiCityUtilities.hpp`](MultiCityUtilities.hpp) | `spectralRatio` computes a matrix's spectral radius. `computeR0` constructs matrices and returns a reproduction-number diagnostic for each city. Its source comments explicitly express uncertainty about the interpretation of the paper's formula. |
| [`RKFMC.hpp`](RKFMC.hpp) | Adaptive embedded Runge–Kutta integration for the model state types, with error estimates, step-size adaptation, and `RKFResult` histories and failure information. |
| [`ButcherRKF.hpp`](ButcherRKF.hpp) | Butcher-table storage and factories for the embedded RK45, RK23, and RK12 schemes. RK45 is the default used by the model advancer. |
| [`RKFTraits.hpp`](RKFTraits.hpp) | Generic scalar, Eigen vector, and Eigen matrix traits for Runge–Kutta state types, forcing functions, and norms. |
| [`GetPot`](GetPot), [`GetPot.hpp`](GetPot.hpp), [`GetPot_impl.hpp`](GetPot_impl.hpp) | Bundled GetPot argument/configuration parser: convenience include, declarations, and implementation respectively. The current demonstration does not use it. |
| [`Makefile`](Makefile) | Builds the example using the shared `../../Makefile.inc`, with an optional local `Makefile.inc` override. Also provides cleaning and Doxygen targets. |

`main_test`, `main_test.o`, and `make.dep` are build artifacts. `result.txt` is
simulation output, regenerated when the executable runs.

## Time integration

`MultiCityModelAdvance::initialData` specifies the initial states, time interval,
number of output intervals (`numSteps`), separate population and epidemic
tolerances, and maximum internal solver steps (`maxSteps`). For each output
interval the advancer:

1. Integrates the population dynamics.
2. Updates the population referenced by the epidemic model to the interval's
   final population.
3. Integrates the epidemic dynamics over that same interval, keeping that
   population fixed.
4. Stores both endpoint states and accumulates solver success information.

The solvers adapt their internal time steps independently. The output interval
therefore also controls the population/epidemic coupling approximation; reducing
the RK tolerances alone does not refine that coupling. `Result::good` reports
whether all solver calls succeeded, while progress and error diagnostics go to
`std::clog`. The demonstration writes its result without checking this flag.

## Build and run

The example requires a C++17-or-newer compiler and Eigen headers. In the PACS
examples tree, configure `../../Makefile.inc` for your compiler and Eigen
installation, then run:

```sh
make
./main_test
```

The shared configuration selects the compiler standard and include paths. For
example, an Eigen installation under `/usr/include/eigen3` can be selected with:

```sh
make mkEigenInc=/usr/include/eigen3
```

Alternatively, this single-source example can be compiled directly, adjusting
the Eigen include path as needed:

```sh
g++ -std=c++17 -O2 -I/usr/include/eigen3 main_test.cpp -o main_test
./main_test
```

There are no command-line settings in the current driver. Change
`main_test.cpp` and rebuild to alter the scenario. It starts with 25,000
residents in each city, all at home, including 100 infectious people in the
second city (C++ index `1`). It loads the two-city coefficients, changes the
first city's contact factor to `2.5`, sets both departure rates to `0.05`, and
enables full immunity after recovery. It advances from time `0` to `1000` in
100 output intervals. The example rates are consistent with time measured in
days; custom parameters must use consistent time units.

The program prints initial states, coefficients, and initial/final `computeR0`
diagnostics. It overwrites `result.txt` in the working directory with:

- An `#Epidemic Data` section: number of saved states, then repeated time values
  and `2n × n` matrices (`S` rows followed by `I` rows).
- An `#Population Data` section: number of saved states, then repeated time
  values and `n × n` population matrices.

A successful default run has 101 states in each section, including the initial
state. The format is plain text with matrix blocks, not one CSV row per time.

To use more cities, instantiate the templates with the desired city count and
provide all initial states and coefficients: the supplied population initializer
and `initializeFromArticle()` implementation cover only the two-city case.
Adding further disease compartments also requires an appropriate epidemic
right-hand side and changes to the advancer/result interfaces, which currently
use the two-compartment result type.
