# Explanation of `string_utility.hpp`

## Overview
The `string_utility.hpp` file provides a collection of utilities for string manipulation and processing. These utilities include trimming, case conversion, file reading, string comparison, and computing string distances. The file also includes a helper class for efficient file reading into memory.

### Key Components

1. **Trimming Functions**
   - `ltrim`: Removes leading whitespace from a string.
   - `rtrim`: Removes trailing whitespace from a string.
   - `trim`: Removes both leading and trailing whitespace from a string.

2. **Case Conversion Functions**
   - `toupper`: Converts all characters in a string to uppercase.
   - `tolower`: Converts all characters in a string to lowercase.

3. **String Comparison**
   - `compareNoCase`: A functor for case-insensitive string comparison.

4. **File Reading Utilities**
   - `nextLine`: Reads the next line from an input stream into a string stream.
   - `cleanStream`: Cleans up an input stream to remove unwanted whitespace.
   - `readWholeFile`: Reads the entire content of a file into a string.

5. **`GlobbedTextReader` Class**
   - A helper class for reading an entire text file into a string stream buffer.
   - Supports efficient file reading and provides access to the buffer for further processing.

6. **String Processing Utilities**
   - `chop`: Splits a string stream into a vector of strings, one for each line.
   - `stringDistance`: Computes the Levenshtein distance between two strings, which measures the number of edits required to transform one string into another.

### Example Usage
Below is an example demonstrating the usage of some utilities:

```cpp
#include <iostream>
#include <sstream>
#include <vector>
#include "string_utility.hpp"

int main() {
    // Example 1: Trimming
    std::string str = "   Hello, World!   ";
    std::cout << "Original: '" << str << "'\n";
    std::cout << "Trimmed: '" << Utility::trim(str) << "'\n";

    // Example 2: Case Conversion
    std::string lower = "hello";
    std::string upper = Utility::toupper(lower);
    std::cout << "Uppercase: " << upper << "\n";

    // Example 3: File Reading
    std::istringstream file("Line 1\nLine 2\nLine 3");
    std::string content = Utility::readWholeFile(file);
    std::cout << "File Content: \n" << content << "\n";

    // Example 4: Levenshtein Distance
    std::string a = "kitten";
    std::string b = "sitting";
    unsigned int distance = Utility::stringDistance(a, b);
    std::cout << "Levenshtein Distance: " << distance << "\n";

    return 0;
}
```

### Explanation of the Example
1. **Trimming**:
   - The `trim` function removes leading and trailing whitespace from the string.

2. **Case Conversion**:
   - The `toupper` function converts all characters in the string to uppercase.

3. **File Reading**:
   - The `readWholeFile` function reads the entire content of a simulated file (using `std::istringstream`) into a string.

4. **Levenshtein Distance**:
   - The `stringDistance` function computes the number of edits required to transform one string into another.

### Output of the Example
```
Original: '   Hello, World!   '
Trimmed: 'Hello, World!'
Uppercase: HELLO
File Content: 
Line 1
Line 2
Line 3
Levenshtein Distance: 3
```

### Advantages
- **Comprehensive String Utilities**: Provides a wide range of functions for string manipulation and processing.
- **Efficient File Reading**: The `GlobbedTextReader` class enables efficient reading of large text files into memory.
- **Levenshtein Distance**: Useful for comparing strings and measuring their similarity.

### Requirements
- **C++11 or Later**: The implementation uses modern C++ features such as lambdas and `std::unique_ptr`.

## Conclusion
The `string_utility.hpp` file offers a versatile set of tools for string manipulation, file reading, and string comparison. Its utilities are designed to be efficient and easy to use, making it a valuable addition to any C++ project.