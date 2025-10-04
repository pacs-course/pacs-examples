# Explanation of `StatisticsComputations.hpp`

## Overview
The `StatisticsComputations.hpp` file provides a collection of utilities for performing statistical computations on datasets. It includes classes and functions for calculating mean, variance, skewness, kurtosis, and other statistical measures. The utilities are designed to handle data incrementally and efficiently, making them suitable for large datasets.

### Key Components

1. **`DataHolder` Class**
   - A utility class for computing mean and variance using a naive algorithm.
   - Supports adding and removing data points incrementally.
   - Uses a shift parameter (`K`) to reduce round-off errors.

2. **`shifted_data_mean_variance` Function**
   - Computes the mean and variance of a dataset using the `DataHolder` class.
   - Designed for use with containers like `std::vector` or `std::list`.

3. **`WelfordAlgorithm` Class**
   - Implements Welford's algorithm for incremental computation of mean, variance, skewness, and kurtosis.
   - Provides an `AggregatedOutput` struct to store the computed statistics.
   - Handles bias correction for kurtosis.

4. **`normalizeInPlace` Function**
   - Normalizes a dataset to the range `[0, 1]` in place.
   - Returns the minimum and maximum values of the original dataset.

5. **`resetStatisticsInPlace` Function**
   - Resets normalized statistics to their original scale using the min and max values.

### Example Usage
Below is an example demonstrating the usage of the utilities:

```cpp
#include <iostream>
#include <vector>
#include "StatisticsComputations.hpp"

int main() {
    std::vector<double> data = {1.0, 2.0, 3.0, 4.0, 5.0};

    // Compute mean and variance using shifted_data_mean_variance
    auto result = apsc::Statistics::shifted_data_mean_variance(data);
    std::cout << "Mean: " << result[0] << ", Variance: " << result[1] << std::endl;

    // Use WelfordAlgorithm for incremental statistics
    apsc::Statistics::WelfordAlgorithm welford;
    for (double x : data) {
        welford.update(x);
    }
    auto stats = welford.finalize();
    std::cout << "Welford Mean: " << stats.mean << ", Variance: " << stats.variance << std::endl;

    // Normalize the data
    auto minmax = apsc::Statistics::normalizeInPlace(data);
    std::cout << "Normalized Data: ";
    for (double x : data) {
        std::cout << x << " ";
    }
    std::cout << std::endl;

    // Reset the statistics to the original scale
    apsc::Statistics::resetStatisticsInPlace(stats, minmax);
    std::cout << "Reset Mean: " << stats.mean << ", Reset Variance: " << stats.variance << std::endl;

    return 0;
}
```

### Explanation of the Example
1. **Mean and Variance**:
   - The `shifted_data_mean_variance` function computes the mean and variance of the dataset.

2. **Welford's Algorithm**:
   - The `WelfordAlgorithm` class is used to compute incremental statistics, including mean and variance.

3. **Normalization**:
   - The `normalizeInPlace` function scales the data to the range `[0, 1]`.

4. **Reset Statistics**:
   - The `resetStatisticsInPlace` function reverts the normalized statistics to their original scale using the min and max values.

### Output of the Example
```
Mean: 3, Variance: 2.5
Welford Mean: 3, Variance: 2.5
Normalized Data: 0 0.25 0.5 0.75 1 
Reset Mean: 3, Reset Variance: 2.5
```

### Advantages
- **Incremental Computation**: The `DataHolder` and `WelfordAlgorithm` classes allow for efficient, incremental updates to statistics.
- **Normalization and Reset**: The utilities provide functions for normalizing data and resetting statistics to their original scale.
- **Comprehensive Statistics**: The `WelfordAlgorithm` class computes advanced statistics like skewness and kurtosis.

### Requirements
- **C++17 or Later**: The implementation uses modern C++ features such as structured bindings and `std::minmax_element`.

## Conclusion
The `StatisticsComputations.hpp` file provides a robust set of tools for statistical analysis. Its incremental computation capabilities and support for normalization make it a valuable utility for processing large datasets efficiently.