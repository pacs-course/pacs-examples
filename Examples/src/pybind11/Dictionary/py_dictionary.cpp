#ifndef HH_DICTIONARY
#define HH_DICTIONARY
#include <pybind11/pybind11.h>
#include <pybind11/pytypes.h> // For pybind11::str
#include <pybind11/stl.h> // For working with STL containers like std::map, std::vector
#include <iostream>
#include <map>
#include <string>
#include <vector>
namespace py = pybind11;
/*!
@file dictionary.cpp
@brief Process a Python dictionary in C++.
@details This file contains a function that processes a Python dictionary in
C++. The dictionary is of the form \code{.py} data = { "name": "string", "age":
integer, "height": double, "is_student": boolean, "courses": std::vector<string>
}
\endcode

*/
void
process_dict(const py::dict &data)
{
  for(auto item : data)
    {
      using namespace std::string_literals;
      // Extract key and value
      std::string key = py::str(item.first); // Convert key to string
      py::object  value = py::reinterpret_borrow<py::object>(
        item.second); // Value as a generic Python object
      if(key == "name")
        {
          std::string val = value.cast<std::string>();
          std::cout << key << ": " << val << " (string)" << std::endl;
        }
      else if(key == "age")
        {
          int val = value.cast<int>();
          std::cout << key << ": " << val << " (int)" << std::endl;
        }
      else if(key == "height")
        {
          double val = value.cast<double>();
          std::cout << key << ": " << val << " (float)" << std::endl;
        }
      else if(key == "is_student")
        {
          bool val = value.cast<bool>();
          std::cout << key << ": " << std::boolalpha << val << " (bool)"
                    << std::endl;
        }
      else if(key == "courses")
        {
          std::vector<py::object> val = value.cast<std::vector<py::object>>();
          std::cout << key << ": [";
          for(const auto &v : val)
            {
              std::cout << py::str(v) << " ";
            }
          std::cout << "] (list)" << std::endl;
        }
      else
        {
          std::cout << key << ": (unknown type)" << std::endl;
        }
    }
}
// write the pybind11 wrapper
PYBIND11_MODULE(dictionary, m)
{
  m.def("process_dict", &process_dict, "Process a Python dictionary",
        py::arg("data"));
}
#endif // HH_DICTIONARY
