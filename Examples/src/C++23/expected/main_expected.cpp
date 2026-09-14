// expected.cpp
/*
@file expected.cpp
@brief This file demonstrates the use of std::expected in C++23.

@link https://www.modernescpp.com/index.php/c23-a-new-way-of-error-handling-with-stdexpected/
*/
#include <iostream>
#include <expected>
#include <vector>
#include <string>

/**
 * @brief A function that attempts to convert a string to an integer.
 * @param arg The string to convert.
 * @return An expected value containing the integer if successful, or an error message if not.
 */
std::expected<int, std::string> getInt(std::string arg) {
    try {
        return std::stoi(arg);
    }
    catch (...) {
        return std::unexpected{std::string(arg + ": Error")};
    }
}

 
int main() {

    std::cout << '\n';

  // Test the getInt function with a vector of strings.
    std::vector<std::string> strings = {"66", "foo", "-5"};

    for (auto s: strings) {                                 // (1)
        auto res = getInt(s);
        if (res) {// std::expected object converts to bool, true if it contains a value, false if it contains an error
            std::cout << res.value() << ' ';                // (3)
        }
        else {
            std::cout << res.error() << ' ';                // (4)
        }
    }

    std::cout << '\n';

    for (auto s: strings) {                                 // (2)
        auto res = getInt(s);
        // Use value_or to provide a default value if the expected object contains an error
        std::cout << res.value_or(2023) << ' ';             // (5)
    }

    std::cout << '\n';

}