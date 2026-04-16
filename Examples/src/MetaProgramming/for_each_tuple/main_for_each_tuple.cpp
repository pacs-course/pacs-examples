#include "for_each_tuple.hpp"
#include <cassert>
#include <cmath>
#include <iostream>
#include <sstream>
#include <tuple>
#include <utility>
#include <vector>

// Test helper: capture output
class OutputCapture
{
public:
  OutputCapture() : old_cout(std::cout.rdbuf(buffer.rdbuf())) {}
  ~OutputCapture() { std::cout.rdbuf(old_cout); }
  std::string
  str() const
  {
    return buffer.str();
  }

private:
  std::stringstream buffer;
  std::streambuf   *old_cout;
};

// Test 1: for_each with simple tuple of integers
int
test_for_each_integers()
{
  auto print_int = [](int x) { std::cout << x << " "; };
  auto tup = std::make_tuple(1, 2, 3, 4, 5);

  {
    OutputCapture capture;
    apsc::for_each(tup, print_int);
    assert(capture.str() == "1 2 3 4 5 ");
  }
  return 0;
}

// Test 2: for_each with mixed types
int
test_for_each_mixed_types()
{
  auto print_val = [](auto x) { std::cout << x << "|"; };
  auto tup = std::make_tuple(42, 3.14, true, 99);

  {
    OutputCapture capture;
    apsc::for_each(tup, print_val);
    std::string output = capture.str();
    assert(output.find("42") != std::string::npos);
    assert(output.find("3.14") != std::string::npos);
    assert(output.find("1") != std::string::npos);
    assert(output.find("99") != std::string::npos);
  }
  return 0;
}

// Test 3: for_each with empty tuple
int
test_for_each_empty_tuple()
{
  auto dummy_func = [](auto x) { std::cout << x; };
  auto tup = std::make_tuple();

  {
    OutputCapture capture;
    apsc::for_each(tup, dummy_func);
    assert(capture.str() == "");
  }
  return 0;
}

// Test 4: for_each with single element
int
test_for_each_single_element()
{
  auto print_val = [](auto x) { std::cout << x << "!"; };
  auto tup = std::make_tuple(100);

  {
    OutputCapture capture;
    apsc::for_each(tup, print_val);
    assert(capture.str() == "100!");
  }
  return 0;
}

// Test 5: for_each with accumulator pattern
int
test_for_each_accumulator()
{
  auto tup = std::make_tuple(1, 2, 3, 4, 5);
  int  sum = 0;
  auto accumulate = [&sum](int x) { sum += x; };

  apsc::for_each(tup, accumulate);
  assert(sum == 15);
  return 0;
}

// Test 6: for_each with strings
int
test_for_each_strings()
{
  auto print_string = [](const std::string &s) {
    std::cout << "[" << s << "]";
  };
  auto tup = std::make_tuple(std::string("hello"), std::string("world"));

  {
    OutputCapture capture;
    apsc::for_each(tup, print_string);
    assert(capture.str() == "[hello][world]");
  }
  return 0;
}

// Test 7: for_each with vectors
int
test_for_each_vectors()
{
  std::vector<int> results;
  auto             collect = [&results](int x) { results.push_back(x); };
  auto             tup = std::make_tuple(10, 20, 30);

  apsc::for_each(tup, collect);
  assert(results.size() == 3);
  assert(results[0] == 10);
  assert(results[1] == 20);
  assert(results[2] == 30);
  return 0;
}

// Test 8: for_each with function pointers
void
print_double(double d)
{
  std::cout << d << " ";
}

int
test_for_each_function_pointers()
{
  auto tup = std::make_tuple(1.5, 2.5, 3.5);

  {
    OutputCapture capture;
    apsc::for_each(tup, print_double);
    assert(capture.str().find("1.5") != std::string::npos);
    assert(capture.str().find("2.5") != std::string::npos);
    assert(capture.str().find("3.5") != std::string::npos);
  }
  return 0;
}

// Test 9: for_each with boolean operations
int
test_for_each_boolean_operations()
{
  auto tup = std::make_tuple(true, false, true, false, true);
  int  true_count = 0;
  auto count_true = [&true_count](bool b) {
    if(b)
      true_count++;
  };

  apsc::for_each(tup, count_true);
  assert(true_count == 3);
  return 0;
}

// Test 10: for_each with const reference lambda
int
test_for_each_const_reference()
{
  auto print_ref = [](const auto &x) { std::cout << x << ","; };
  auto tup = std::make_tuple(7, 8, 9);

  {
    OutputCapture capture;
    apsc::for_each(tup, print_ref);
    assert(capture.str() == "7,8,9,");
  }
  return 0;
}

// Main test runner
int
main()
{
  int error_count = 0;

  try
    {
      error_count += test_for_each_integers();
      std::cout << "Test 1 (integers): PASSED\n";
    }
  catch(...)
    {
      std::cout << "Test 1 (integers): FAILED\n";
      error_count++;
    }

  try
    {
      error_count += test_for_each_mixed_types();
      std::cout << "Test 2 (mixed types): PASSED\n";
    }
  catch(...)
    {
      std::cout << "Test 2 (mixed types): FAILED\n";
      error_count++;
    }

  try
    {
      error_count += test_for_each_empty_tuple();
      std::cout << "Test 3 (empty tuple): PASSED\n";
    }
  catch(...)
    {
      std::cout << "Test 3 (empty tuple): FAILED\n";
      error_count++;
    }

  try
    {
      error_count += test_for_each_single_element();
      std::cout << "Test 4 (single element): PASSED\n";
    }
  catch(...)
    {
      std::cout << "Test 4 (single element): FAILED\n";
      error_count++;
    }

  try
    {
      error_count += test_for_each_accumulator();
      std::cout << "Test 5 (accumulator): PASSED\n";
    }
  catch(...)
    {
      std::cout << "Test 5 (accumulator): FAILED\n";
      error_count++;
    }

  try
    {
      error_count += test_for_each_strings();
      std::cout << "Test 6 (strings): PASSED\n";
    }
  catch(...)
    {
      std::cout << "Test 6 (strings): FAILED\n";
      error_count++;
    }

  try
    {
      error_count += test_for_each_vectors();
      std::cout << "Test 7 (vectors): PASSED\n";
    }
  catch(...)
    {
      std::cout << "Test 7 (vectors): FAILED\n";
      error_count++;
    }

  try
    {
      error_count += test_for_each_function_pointers();
      std::cout << "Test 8 (function pointers): PASSED\n";
    }
  catch(...)
    {
      std::cout << "Test 8 (function pointers): FAILED\n";
      error_count++;
    }

  try
    {
      error_count += test_for_each_boolean_operations();
      std::cout << "Test 9 (boolean operations): PASSED\n";
    }
  catch(...)
    {
      std::cout << "Test 9 (boolean operations): FAILED\n";
      error_count++;
    }

  try
    {
      error_count += test_for_each_const_reference();
      std::cout << "Test 10 (const reference): PASSED\n";
    }
  catch(...)
    {
      std::cout << "Test 10 (const reference): FAILED\n";
      error_count++;
    }

  std::cout << "\n" << (10 - error_count) << "/10 tests passed\n";
  return error_count;
}
