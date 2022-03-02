#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include <vector>

// ----------------
// Regular C++ code
// ----------------

// Multiply all entries by 2.0.
// input:  std::vector ([...]) (read only)
// output: std::vector ([...]) (new copy)
std::vector<double>
modify(const std::vector<double> &input)
{
  std::vector<double> output;

  std::transform(input.begin(),
                 input.end(),
                 std::back_inserter(output),
                 [](double x) -> double { return 2. * x; });

  // N.B. this is equivalent to (but there are also other ways to do the same)
  //
  // std::vector<double> output(input.size());

  // for (size_t i = 0; i < input.size(); ++i)
  //   output[i] = 2. * input[i];

  return output;
}

// Create a map given a set of keys and the corresponding values.
// Keys and value types are specified as template parameters.
template <class KeyType, class ValueType>
std::map<KeyType, ValueType>
create_map(const std::set<KeyType> &keys, const std::vector<ValueType> &values)
{
  assert(keys.size() == values.size());

  std::map<KeyType, ValueType> output;

  size_t i = 0;
  for (const auto &key : keys)
    {
      output[key] = values[i];
      ++i;
    }

  return output;
}

// ----------------
// Python interface
// ----------------

namespace py = pybind11;

PYBIND11_MODULE(example, m)
{
  // Print doc by calling
  // help(example)
  // from Python.
  m.doc() = "pybind11 example plugin";

  // Print help by calling
  // help(example.modify)
  // from Python.
  m.def("modify", &modify, "Multiply all entries of a list by 2.0");

  // Binding templates requires explicit instantiation.
  // A std::map is bound into a Python dictionary.
  // A std::set is bound into a Python set.
  m.def("create_map",
        &create_map<int, double>,
        "Create a map of double numbers from a list of integer keys");
  m.def("create_map",
        &create_map<std::string, int>,
        "Create a map of integer numbers from a list of string keys");
}
