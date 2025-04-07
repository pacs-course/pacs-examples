# Usage of the random distribution utilities available in the Standard library #


The file `main_random.cpp` produces a set of samples generated with
different distributions. It then generates a set of data according to that distribution and computes some statistics of the sample.

It also plots an istogram of each sample using the gnuplot-iostream facility.

**Note** You must have gnuplot and the boost libraries installed in your system
    to use gnuplot-iostream. If not you can add `CPPFLAGS+=-DNO_GNUPLOT` in `Makefile.inc` and the istogram data will be printed on the screen instead.

It also some files with extension `.dat` with the values produced by
each distribution. The number of samples may be changed by setting the
constexpr variable `N` in the source file.

The data file produced by main_random may be seen with `gnuplot`. You only
need to skip the first line, which contains the number of samples.
You can use this trick, at the glupolt prompt

    gnuplot> set key autotitle columnhead
    gnuplot> plot "filename" w p title "samples"

Or

    gnuplot>plot "filename"  every ::1

The code uses the header `StatisticsComputations.hpp`, which is in the `Utility` folder. You have to install it (following the instructions in that folder).

## How to run the code ##
1. Make sure you have gnuplot and boost libraries installed in your system.
2. If you are using the modules, load the boost module with `module load boost`.
3. If you are not using the modules, modify `Makefile.inc` to suit your needs.
4. Compile the code with `make` (or `make -f Makefile.inc` if you are not using the modules).

If you type 

```bash
./main_random -h
```
you have a list of the options available. The default is to generate 150000 samples for each distribution, but you can change it with the `-s` option.

## A note on the convergence of higher order moments ##
The code computes the mean, variance, skewness and kurtosis of the samples. We use the **Welford** algorithm to compute the statistics incrementally, as implemented in `Utilities/StatisticsComputations.hpp`.
The algorithm is numerically stable. However, the convergence of the higher order moments is not guaranteed. In particular, kurtosis is not guaranteed to converge to the correct value for all distributions. This is a known issue and is related to sensitivity to sampling variability. The sample kurtosis estimator is biased because it does not account for the variability introduced by finite sample sizes. Specifically:

For small sample sizes, the sample kurtosis tends to overestimate the true kurtosis.
As the sample size increases, the bias becomes negative, causing the computed excess kurtosis to approach `-3` for most distribution (including Gaussian). This bias arises because the sample kurtosis formula assumes that the sample moments (e.g., variance, skewness) are perfect estimates of the population moments, which is not true for finite samples.

A bias correction formula for kurtosis has been applied, but apparently it does not do much. It may be noted that the best approximation for kurtosis is obtained with a resonable sized, but not too large sample (arount 10000). The sample size of 150000 is too large for the kurtosis to be reliable.

A possible solution if to implement batch sampling: split the sample in batches of 1000 and compute the statistics for each batch. Then combine the results using the Welford algorithm. This is not implemented in this code, but it is a possible improvement.

Another problem is the accumulation of round-off errors. I have mitigated it usiing `long double` in `StatisticsComputations.hpp`, but it is not a complete solution. I have alos sorted the data in ascienfing order before computing the statistics. This is not necessary for the Welford algorithm, but it helps to reduce the round-off errors.

Data normalization can reduce rond-off errors in some cases, in `StatisticsComputations.hpp` you have the tool to nomalize data and to recover the original statistics form those computed on the normalized data. This is not implemented in this code, but it is a possible improvement. 


# What do I learn here? #
- The different random number generators and engines of the standard library
- An utility to extract statistical information from a sample (univariate)
- A use of `gnuplot-iostream`
- The effect of bias and round-off errors on the computation of higher order moments

## Important change since the last version (April 2024) ##
All distributions are in fact template class with default parametres. Since you are normally happy with the defaults
you can use the distribution omitting the parameters. However, you can (since C++17) operate in two ways:
For instance if you write 

```cpp
    std::lognormal_distribution<> lnd{3.,2.};
```
you are requesting a lognormal distribution with the default template parameter, which is 
    
```cpp
    std::lognormal_distribution<double>
```
You can also write

```cpp
    std::lognormal_distribution lnd{3., 2.};
```
which is equivalent to the previous one. The difference is that in the first case you are specifying the type of the distribution, in the second case you are not. The compiler will infer the type from the parameters you are passing in the constructor, and thus produce `std::lognormal_distribution<double>`. This is a new feature of C++17.

However if you forget the dots
```cpp
    std::lognormal_distribution lnd{3, 2};
```
The compiler generates a `std::lognormal_distribution<int>` and gives a compilation error since the tempalte argument for this can be only a floating point type. This does not happen with 

```cpp
std::lognormal_distribution<> lnd{3, 2};
```
as in this case the compiler infers the type from the parameters and generates a `std::lognormal_distribution<double>`: the two ints are converted to double.

On the other hand if you write

```cpp
    std::lognormal_distribution lnd{3.0f,2.0f};
```
the compiler generates a `std::lognormal_distribution<float>` since the constructor areguments are float.
While, if you do 
```cpp
    std::lognormal_distribution<> lnd{3.0f,2.0f};
```
the compiler generates a `std::lognormal_distribution<double>` since the default template parameter is double!
If you do not want to use the automatic template deduction, in this case you have to do
```cpp
   std::lognormal_distribution<float> lnd{3.0f, 2.0f};
```
if you really want to use the version for `floats`

The message I want to give you in this note is that selecting all the default template parameters requires to indicate `<>`. You can avoid it (since c++17) if you can use the new automatic parameter deduction feature for class templates, but then remember that the deduced template parameters depend on the type of the constructor arguments. 

## Important ##

`gnoplot-iostream` uses the boost libraries. So if you want the
example to be working you must have the boost library (development
version) installed. Not all of them infact, in Makefile.inc you see the one that are required: `boos-iostream`, `boost-system` and `boost-filesystem`.

If you are using the modules you have all of them with

```
module load boost
```

If you do not use the modules, modify `Makefile.inc` to suit your need!.
